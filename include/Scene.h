#pragma once

#include "Model.h"
#include "ShaderProgram.h"
#include "DrawableObject.h"
#include "Camera.h"
#include "Light.h"
#include "Material.h"

#include <cstddef>
#include <memory>
#include <vector>

class Scene {
public:
	Scene() = default;
	~Scene();

	Scene(const Scene&) = delete;
	Scene& operator=(const Scene&) = delete;

	void configureCamera(float alphaDegrees, float fiDegrees, const glm::vec3& position, float fieldOfViewDegrees);
	void configureLight(const glm::vec3& position, const glm::vec3& color, float constant, float linear, float quadratic);

	void rotateCamera(float deltaX, float deltaY);
	void resizeCamera(int width, int height);
	void moveCamera(float forwardInput, float rightInput, float deltaTime);

	Model& addModel(const float* vertices, std::size_t floatCount, VertexFormat format, GLenum drawingMode = GL_TRIANGLES);
	Material& addMaterial();
	ShaderProgram& addShaderProgram(const Shader& vertexShader, const Shader& fragmentShader, bool useCamera = true, bool useLighting = false);
	DrawableObject& addObject(Model& model, ShaderProgram& shaderProgram, Material& material);

	void draw(double timeSeconds);
	void clear();

private:
	Camera camera;
	Light light;

	std::vector<std::unique_ptr<Model>> models;
	std::vector<std::unique_ptr<Material>> materials;
	std::vector<std::unique_ptr<ShaderProgram>> shaderPrograms;
	std::vector<std::unique_ptr<DrawableObject>> objects;
};