#include "DrawableObject.h"
#include "Model.h"
#include "ShaderProgram.h"

#include <GLFW/glfw3.h>

DrawableObject::DrawableObject(Model& model, ShaderProgram& shaderProgram) : model(model), shaderProgram(shaderProgram)
{
}

void DrawableObject::setTranslation(float x, float y, float z)
{
	transformation.setTranslation(x, y, z);
}

void DrawableObject::setScale(float value)
{
	transformation.setScale(value);
}

void DrawableObject::setRotationAngle(float angle)
{
	transformation.setRotationAngle(angle);
}

void DrawableObject::draw() const
{
	shaderProgram.use();
	transformation.applyTo(shaderProgram);
	model.draw();

	glUseProgram(0);
}
