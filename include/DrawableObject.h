#pragma once

#include <memory>

class Model;
class Material;
class ShaderProgram;
class ITransformation;

class DrawableObject {
public:
	DrawableObject(Model& model, ShaderProgram& shaderProgram, Material& material);
	~DrawableObject();

	void setTransformation(std::unique_ptr<ITransformation> value);
	void draw(double timeSeconds) const;

private:
	Model& model;
	ShaderProgram& shaderProgram;
	Material& material;

	std::unique_ptr<ITransformation> transformation;
};