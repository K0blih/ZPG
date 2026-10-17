#pragma once

#include "ICameraObserver.h"

#include <glad/gl.h>
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>

class Shader;

class ShaderProgram : public ICameraObserver {
public:
	ShaderProgram() = default;
	~ShaderProgram();

	ShaderProgram(const ShaderProgram&) = delete;
	ShaderProgram& operator=(const ShaderProgram&) = delete;
	
	void link(const Shader& vertex, const Shader& fragment);
	void use() const;
	void reset();

	void setUniform(const char* name, float value) const;
	void setUniform(const char* name, const glm::vec3& value) const;
	void setUniform(const char* name, const glm::mat4& value) const;

	void onCameraChanged(const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix) override;

private:
	GLint getUniformLocation(const char* name) const;

	GLuint shaderProgramId = 0;

	glm::mat4 cameraView{ 1.0f };
	glm::mat4 cameraProjection{ 1.0f };
};