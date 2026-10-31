#pragma once

#include "ITransformation.h"

#include <glm/vec3.hpp>

class DynamicTransformation final : public ITransformation {
public:
    DynamicTransformation(float degreesPerSecond, const glm::vec3& axis, float initialAngleDegrees = 0.0f);
    DynamicTransformation(float degreesPerSecond, float x, float y, float z, float initialAngleDegrees = 0.0f);

    glm::mat4 getMatrix(double timeSeconds) const override;

private:
    float degreesPerSecond;
    glm::vec3 axis;
    float initialAngleDegrees;
};
