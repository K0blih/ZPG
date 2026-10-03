#pragma once

#include "Model.h"
#include "ShaderProgram.h"
#include "DrawableObject.h"

#include <cstddef>
#include <memory>
#include <vector>

class Scene {
public:
	Scene() = default;
	~Scene();

	Scene(const Scene&) = delete;
	Scene& operator=(const Scene&) = delete;

	Model& addModel(const float* vertices, std::size_t floatCount, GLenum drawingMode = GL_TRIANGLES);
	ShaderProgram& addShaderProgram(const Shader& vertexShader, const Shader& fragmentShader);
	DrawableObject& addObject(Model& model, ShaderProgram& shaderProgram);

	void createTriangle();
	void createSphere();
	void createForest();
	void createLogin();
	void draw();
	void clear();

private:
	std::vector<std::unique_ptr<Model>> models;
	std::vector<std::unique_ptr<ShaderProgram>> shaderPrograms;
	std::vector<std::unique_ptr<DrawableObject>> objects;

	// will be removed in the future, just for testing purposes
	bool spinning = false;
	float rotationAngle = 0.0f;
};