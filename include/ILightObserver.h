#pragma once

#include <glm/vec3.hpp>

class ILightObserver {
public:
    virtual ~ILightObserver() = default;

    virtual void onLightChanged(const glm::vec3& position, const glm::vec3& color, float constant, float linear, float quadratic) = 0;
};