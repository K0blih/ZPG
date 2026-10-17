#pragma once

#include <glm/glm.hpp>
#include <vector>

class ICameraObserver;

class Camera {
public:
    Camera();

    Camera(const Camera&) = delete;
    Camera& operator=(const Camera&) = delete;

    void subscribe(ICameraObserver& observer);
    void unsubscribe(ICameraObserver& observer);

    void setAngles(float alphaDegrees, float fiDegrees);

    void rotate(float deltaX, float deltaY);
    void resize(int width, int height);
    void move(float forwardInput, float rightInput, float deltaTime);

private:
    void updateView();
    void notify();

    glm::vec3 eye{0.0f, 0.0f, 3.0f};
    glm::vec3 target{0.0f, 0.0f, -1.0f};
    glm::vec3 up{0.0f, 1.0f, 0.0f};

    float alpha = 90.0f;
    float fi = 270.0f;

    float fieldOfView = 60.0f;
    float mouseSensitivity = 0.15f;
	float movementSpeed = 3.0f;

    glm::mat4 viewMatrix{1.0f};
    glm::mat4 projectionMatrix{1.0f};

    std::vector<ICameraObserver*> observers;
};