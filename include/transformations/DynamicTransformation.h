#pragma once

#include "ITransformation.h"

#include <glm/vec3.hpp>

class DynamicTransformation final : public ITransformation {
public:
    DynamicTransformation(float radiansPerSecond, const glm::vec3& axis, float initialAngle = 0.0f);
    DynamicTransformation(float radiansPerSecond, float x, float y, float z, float initialAngle = 0.0f);

    glm::mat4 getMatrix(double timeSeconds) const override;

private:
    float radiansPerSecond;
    glm::vec3 axis;
    float initialAngle;
};
