#pragma once

#include <Transformation.h>

class Model;
class ShaderProgram;

class DrawableObject {
public:
	DrawableObject(Model& model, ShaderProgram& shaderProgram);

	void setTranslation(float x, float y, float z);
	void setScale(float value);
	void setRotationAngle(float angle);

	void draw() const;

private:
	Model& model;
	ShaderProgram& shaderProgram;
	Transformation transformation;
};