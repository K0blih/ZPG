#pragma once

#include <glm/mat4x4.hpp>

class Transformation {
public:
    virtual ~Transformation() = default;

    virtual glm::mat4 getMatrix(double timeSeconds) const = 0;
};