#pragma once

#include "Transformation.h"

#include <memory>
#include <vector>

class CompositeTransformation final : public Transformation {
public:
    void add(std::unique_ptr<Transformation> child);

    glm::mat4 getMatrix(double timeSeconds) const override;

private:
    std::vector<std::unique_ptr<Transformation>> children;
};