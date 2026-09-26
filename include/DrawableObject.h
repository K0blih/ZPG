#pragma once

class Model;
class ShaderProgram;

class DrawableObject {
public:
	DrawableObject(Model& model, ShaderProgram& shaderProgram);

	void draw() const;

private:
	Model& model;
	ShaderProgram& shaderProgram;
};