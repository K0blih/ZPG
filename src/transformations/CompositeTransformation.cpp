#include "transformations/CompositeTransformation.h"

#include <stdexcept>
#include <utility>

void CompositeTransformation::add(std::unique_ptr<Transformation> child)
{
	if (!child)
	{
		throw std::invalid_argument("Child transformation cannot be null");
	}

	children.push_back(std::move(child));
}

glm::mat4 CompositeTransformation::getMatrix(double timeSeconds) const
{
	glm::mat4 result(1.0f);
	for (const auto& child : children)
	{
		result *= child->getMatrix(timeSeconds);
	}

	return result;
}