#include "transformations/DynamicTransformation.h"

#include <glm/geometric.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <stdexcept>

DynamicTransformation::DynamicTransformation(float radiansPerSecond, const glm::vec3& axis, float initialAngle)
	: radiansPerSecond(radiansPerSecond), axis(axis), initialAngle(initialAngle)
{
	if (glm::dot(axis, axis) == 0.0f)
	{
		throw std::invalid_argument("Axis vector cannot be zero");
	}

	this->axis = glm::normalize(axis);
}

DynamicTransformation::DynamicTransformation(float radiansPerSecond, float x, float y, float z, float initialAngle)
	: DynamicTransformation(radiansPerSecond, glm::vec3(x, y, z), initialAngle)
{
}

glm::mat4 DynamicTransformation::getMatrix(double timeSeconds) const
{
	const double angle = initialAngle + radiansPerSecond * timeSeconds;

	return glm::rotate(glm::mat4(1.0f), static_cast<float>(angle), axis);
}
