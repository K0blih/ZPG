#pragma once

#include <memory>

class Model;
class ShaderProgram;
class ITransformation;

class DrawableObject {
public:
	DrawableObject(Model& model, ShaderProgram& shaderProgram);
	~DrawableObject();

	void setTransformation(std::unique_ptr<ITransformation> value);
	void draw(double timeSeconds) const;

private:
	Model& model;
	ShaderProgram& shaderProgram;
	std::unique_ptr<ITransformation> transformation;
};