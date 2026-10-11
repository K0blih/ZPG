#pragma once

#include <glm/mat4x4.hpp>

class ITransformation {
public:
    virtual ~ITransformation() = default;

    virtual glm::mat4 getMatrix(double timeSeconds) const = 0;
};
