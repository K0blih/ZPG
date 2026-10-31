#include "DrawableObject.h"
#include "Model.h"
#include "ShaderProgram.h"
#include "transformations/ITransformation.h"
#include "Material.h"

#include <GLFW/glfw3.h>
#include <utility>

DrawableObject::DrawableObject(Model& model, ShaderProgram& shaderProgram, Material& material) : model(model), shaderProgram(shaderProgram), material(material)
{
}

void DrawableObject::setTransformation(std::unique_ptr<ITransformation> value)
{
	transformation = std::move(value);
}
	
void DrawableObject::draw(double timeSeconds) const
{
	const glm::mat4 modelMatrix = transformation ? transformation->getMatrix(timeSeconds) : glm::mat4(1.0f);
	shaderProgram.setUniform("modelMatrix", modelMatrix);

	material.apply(shaderProgram);
	model.draw();
}

DrawableObject::~DrawableObject() = default;
