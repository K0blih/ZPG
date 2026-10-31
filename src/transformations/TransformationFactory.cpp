#include "transformations/TransformationFactory.h"

std::unique_ptr<StaticTransformation>TransformationFactory::makeTranslation(float x, float y, float z)
{
    auto transformation = std::make_unique<StaticTransformation>();
    transformation->translate(x, y, z);
    return transformation;
}

std::unique_ptr<StaticTransformation>TransformationFactory::makeScale(float x, float y, float z)
{
    auto transformation = std::make_unique<StaticTransformation>();
    transformation->scale(x, y, z);
    return transformation;
}

std::unique_ptr<StaticTransformation>TransformationFactory::makeScale(float value)
{
    return makeScale(value, value, value);
}

std::unique_ptr<StaticTransformation>TransformationFactory::makeRotation(float angleDegrees, float x, float y, float z)
{
    auto transformation = std::make_unique<StaticTransformation>();
    transformation->rotate(angleDegrees, x, y, z);
    return transformation;
}

std::unique_ptr<StaticTransformation> TransformationFactory::makeStatic(const glm::mat4& matrix)
{
    return std::make_unique<StaticTransformation>(matrix);
}

std::unique_ptr<DynamicTransformation>TransformationFactory::makeDynamicRotation(
    float degreesPerSecond, float x, float y, float z, float initialAngleDegrees)
{
    return std::make_unique<DynamicTransformation>(degreesPerSecond, x, y, z, initialAngleDegrees);
}