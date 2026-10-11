#include "DrawableObject.h"
#include "Model.h"
#include "ShaderProgram.h"
#include "transformations/Transformation.h"

#include <GLFW/glfw3.h>
#include <utility>

DrawableObject::DrawableObject(Model& model, ShaderProgram& shaderProgram) : model(model), shaderProgram(shaderProgram)
{
}

void DrawableObject::setTransformation(std::unique_ptr<Transformation> value)
{
	transformation = std::move(value);
}
	
void DrawableObject::draw(double timeSeconds) const
{
	const glm::mat4 modelMatrix = transformation ? transformation->getMatrix(timeSeconds) : glm::mat4(1.0f);

	shaderProgram.use();
	shaderProgram.setUniform("modelMatrix", modelMatrix);

	model.draw();

	glUseProgram(0);
}

DrawableObject::~DrawableObject() = default;
