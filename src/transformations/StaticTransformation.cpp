#include "transformations/StaticTransformation.h"

#include <glm/geometric.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <stdexcept>

StaticTransformation::StaticTransformation(const glm::mat4& matrix) : matrix(matrix)
{
}

void StaticTransformation::translate(float x, float y, float z)
{
	matrix = glm::translate(matrix, glm::vec3(x, y, z));
}

void StaticTransformation::scale(float x, float y, float z)
{
	matrix = glm::scale(matrix, glm::vec3(x, y, z));
}

void StaticTransformation::scale(float value)
{
	scale(value, value, value);
}

void StaticTransformation::rotate(float angleDegrees, const glm::vec3& axis)
{
	if (glm::dot(axis, axis) == 0.0f)
	{
		throw std::invalid_argument("Axis vector cannot be zero");
	}

	matrix = glm::rotate(matrix, glm::radians(angleDegrees), glm::normalize(axis));
}

void StaticTransformation::rotate(float angleDegrees, float x, float y, float z)
{
	rotate(angleDegrees, glm::vec3(x, y, z));
}

glm::mat4 StaticTransformation::getMatrix(double timeSeconds) const
{
	return matrix;
}