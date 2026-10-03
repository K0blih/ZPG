#pragma once

#include <glm/vec3.hpp>

class ShaderProgram;

class Transformation {
public:
	Transformation() = default;
	~Transformation() = default;

	void setTranslation(float x, float y, float z);
	void setScale(float value);
	void setRotationAngle(float angle);

	void applyTo(const ShaderProgram& shaderProgram) const;

private:
	glm::vec3 translation{0.0f};
	float scale = 1.0f;
	float rotationAngle = 0.0f;
};