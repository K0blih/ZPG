#include "Transformation.h"
#include "ShaderProgram.h"

void Transformation::setTranslation(float x, float y, float z)
{
	translation = glm::vec3(x, y, z);
}

void Transformation::setScale(float value)
{
	scale = value;
}

void Transformation::setRotationAngle(float angle)
{
	rotationAngle = angle;
}

void Transformation::applyTo(const ShaderProgram& shaderProgram) const
{
	shaderProgram.setUniform("scale", scale);
	shaderProgram.setUniform("translation", translation);
	shaderProgram.setUniform("rotationAngle", rotationAngle);
}
