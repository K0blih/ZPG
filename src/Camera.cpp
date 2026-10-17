#include "Camera.h"
#include "ICameraObserver.h"

#include <glm/gtc/matrix_transform.hpp>

#include <algorithm>
#include <cmath>

Camera::Camera()
{
    updateView();
}

void Camera::subscribe(ICameraObserver& observer)
{
    const auto it = std::find(observers.begin(), observers.end(), &observer);

    if (it == observers.end()) {
        observers.push_back(&observer);
    }

    // Send the current matrices immediately.
    observer.onCameraChanged(viewMatrix, projectionMatrix);
}

void Camera::unsubscribe(ICameraObserver& observer)
{
    std::erase(observers, &observer);
}

void Camera::setAngles(float alphaDegrees, float fiDegrees)
{
    // Avoid looking exactly parallel to the up vector.
    alpha = std::clamp(alphaDegrees, 1.0f, 179.0f);

    // Keep the horizontal angle in [0, 360).
    fi = std::fmod(fiDegrees, 360.0f);

    if (fi < 0.0f) {
        fi += 360.0f;
    }

    updateView();
    notify();
}

void Camera::rotate(float deltaX, float deltaY)
{
    setAngles(alpha + deltaY * mouseSensitivity, fi + deltaX * mouseSensitivity);
}

void Camera::resize(int width, int height)
{
    if (width <= 0 || height <= 0) {
        return;
    }

    const float aspectRatio = static_cast<float>(width) / static_cast<float>(height);

    projectionMatrix = glm::perspective(glm::radians(fieldOfView), aspectRatio, 0.1f, 100.0f);

    notify();
}

void Camera::move(float forwardInput, float rightInput, float deltaTime)
{
    if (deltaTime <= 0.0f || (forwardInput == 0.0f && rightInput == 0.0f))
    {
        return;
    }

    // In your Camera, target is the viewing direction.
    const glm::vec3 forward = glm::normalize(target);
    const glm::vec3 right = glm::normalize(glm::cross(forward, up));

    // Normalize so diagonal movement has the same speed.
    const glm::vec3 direction = glm::normalize(forward * forwardInput + right * rightInput);

    eye += direction * movementSpeed * deltaTime;

    updateView();
    notify();
}

void Camera::updateView()
{
    const float alphaRadians = glm::radians(alpha);
    const float fiRadians = glm::radians(fi);

    target.x = std::sin(alphaRadians) * std::cos(fiRadians);
    target.y = std::cos(alphaRadians);
    target.z = std::sin(alphaRadians) * std::sin(fiRadians);

    viewMatrix = glm::lookAt(eye, eye + target, up);
}

void Camera::notify()
{
    for (ICameraObserver* observer : observers) {
        observer->onCameraChanged(viewMatrix, projectionMatrix);
    }
}
