#pragma once

#include "StaticTransformation.h"
#include "DynamicTransformation.h"

#include <memory>

class TransformationFactory final {
public:
    TransformationFactory() = delete;

    static std::unique_ptr<StaticTransformation> makeTranslation(float x, float y, float z);
    static std::unique_ptr<StaticTransformation> makeScale(float x, float y, float z);
    static std::unique_ptr<StaticTransformation> makeScale(float value);
    static std::unique_ptr<StaticTransformation> makeRotation(float angleRadians, float x, float y, float z);
    static std::unique_ptr<DynamicTransformation> makeDynamicRotation(
        float radiansPerSecond, float x, float y, float z, float initialAngle = 0.0f);
};