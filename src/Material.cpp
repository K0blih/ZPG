#include "Material.h"
#include "ShaderProgram.h"
#include  "Texture.h"

#include <stdexcept>
#include <utility>

void Material::setColor(const glm::vec3& value)
{
	color = value;
}

void Material::setAmbientStrength(float value)
{
	if (value < 0.0f) {
		throw std::invalid_argument("Ambient strength must be non-negative");
	}

	ambientStrength = value;
}

void Material::setSpecularStrength(float value)
{
	if (value < 0.0f) {
		throw std::invalid_argument("Specular strength must be non-negative");
	}

	specularStrength = value;
}

void Material::setShininess(float value)
{
	if (value <= 0.0f) {
		throw std::invalid_argument("Shininess must be positive");
	}

	shininess = value;
}

void Material::setUseVertexColor(bool value)
{
	useVertexColor = value;
}

void Material::setTexture(std::shared_ptr<Texture> value)
{
	texture = std::move(value);
}

void Material::apply(ShaderProgram& shaderProgram) const
{
	shaderProgram.setOptionalUniform("objectColor", color);
	shaderProgram.setOptionalUniform("ambientStrength", ambientStrength);
	shaderProgram.setOptionalUniform("specularStrength", specularStrength);
	shaderProgram.setOptionalUniform("shininess", shininess);
	shaderProgram.setOptionalUniform("useVertexColor", useVertexColor ? 1.0f : 0.0f);

	if (texture)
	{
		texture->bind(0);
		shaderProgram.setUniform("textureUnitID", 0);
	}
}
