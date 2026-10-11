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

std::unique_ptr<StaticTransformation>TransformationFactory::makeRotation(float angleRadians, float x, float y, float z)
{
    auto transformation = std::make_unique<StaticTransformation>();
    transformation->rotate(angleRadians, x, y, z);
    return transformation;
}

std::unique_ptr<DynamicTransformation>TransformationFactory::makeDynamicRotation(
    float radiansPerSecond, float x, float y, float z, float initialAngle)
{
    return std::make_unique<DynamicTransformation>(radiansPerSecond, x, y, z, initialAngle);
}