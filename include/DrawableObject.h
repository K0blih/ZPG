#pragma once

#include <memory>

class Model;
class ShaderProgram;
class Transformation;

class DrawableObject {
public:
	DrawableObject(Model& model, ShaderProgram& shaderProgram);
	~DrawableObject();

	void setTransformation(std::unique_ptr<Transformation> value);
	void draw(double timeSeconds) const;

private:
	Model& model;
	ShaderProgram& shaderProgram;
	std::unique_ptr<Transformation> transformation;
};