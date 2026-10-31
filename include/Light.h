#pragma once

#include <glm/vec3.hpp>
#include <vector>

class ILightObserver;

class Light {
public:
    Light() = default;

    Light(const Light&) = delete;
    Light& operator=(const Light&) = delete;

    void subscribe(ILightObserver& observer);
    void unsubscribe(ILightObserver& observer);

    void setPosition(const glm::vec3& value);
    void setColor(const glm::vec3& value);
    void setAttenuation(float constant, float linear, float quadratic);

private:
    void notify();

    glm::vec3 position{0.0f};
    glm::vec3 color{1.0f};

    float attenuationConstant = 1.0f;
    float attenuationLinear = 0.09f;
    float attenuationQuadratic = 0.032f;

    std::vector<ILightObserver*> observers;
};