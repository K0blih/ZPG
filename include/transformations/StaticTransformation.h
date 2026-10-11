#pragma once

#include "Transformation.h"

#include <glm/vec3.hpp>

class StaticTransformation final : public Transformation {
public:
    StaticTransformation() = default;
    explicit StaticTransformation(const glm::mat4& matrix);

	void translate(float x, float y, float z);
	void scale(float x, float y, float z);
	void scale(float value);
	void rotate(float angleRadians, const glm::vec3& axis);
	void rotate(float angleRadians, float x, float y, float z);

    glm::mat4 getMatrix(double timeSeconds) const override;

private:
    glm::mat4 matrix{1.0f};
};
