#include "transformations/DynamicTransformation.h"

#include <glm/geometric.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <stdexcept>

DynamicTransformation::DynamicTransformation(float degreesPerSecond, const glm::vec3& axis, float initialAngleDegrees)
	: degreesPerSecond(degreesPerSecond), axis(axis), initialAngleDegrees(initialAngleDegrees)
{
	if (glm::dot(axis, axis) == 0.0f)
	{
		throw std::invalid_argument("Axis vector cannot be zero");
	}

	this->axis = glm::normalize(axis);
}

DynamicTransformation::DynamicTransformation(float degreesPerSecond, float x, float y, float z, float initialAngleDegrees)
	: DynamicTransformation(degreesPerSecond, glm::vec3(x, y, z), initialAngleDegrees)
{
}

glm::mat4 DynamicTransformation::getMatrix(double timeSeconds) const
{
	const double angleDegrees = initialAngleDegrees + degreesPerSecond * timeSeconds;

	return glm::rotate(glm::mat4(1.0f), glm::radians(static_cast<float>(angleDegrees)), axis);
}
