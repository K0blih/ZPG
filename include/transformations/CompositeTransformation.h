#pragma once

#include "ITransformation.h"

#include <memory>
#include <vector>

class CompositeTransformation final : public ITransformation {
public:
    void add(std::unique_ptr<ITransformation> child);

    glm::mat4 getMatrix(double timeSeconds) const override;

private:
    std::vector<std::unique_ptr<ITransformation>> children;
};