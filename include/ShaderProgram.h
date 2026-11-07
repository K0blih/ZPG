#pragma once

#include "ICameraObserver.h"
#include "ILightObserver.h"

#include <glad/gl.h>
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>

class Shader;
class Material;
class Light;

class ShaderProgram : public ICameraObserver, public ILightObserver {
public:
	ShaderProgram() = default;
	~ShaderProgram();

	ShaderProgram(const ShaderProgram&) = delete;
	ShaderProgram& operator=(const ShaderProgram&) = delete;
	
	void link(const Shader& vertex, const Shader& fragment);
	void use() const;
	static void unuse();
	void reset();

	void setUniform(const char* name, int value) const;
	void setUniform(const char* name, float value) const;
	void setUniform(const char* name, const glm::vec3& value) const;
	void setUniform(const char* name, const glm::mat4& value) const;
	void setOptionalUniform(const char* name, float value) const;
	void setOptionalUniform(const char* name, const glm::vec3& value) const;
	void updateTimeUniform() const;

	void onCameraChanged(const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix, const glm::vec3& position) override;
	void onLightChanged(const glm::vec3& position, const glm::vec3& color, float constant, float linear, float quadratic) override;

private:
	GLint findUniformLocation(const char* name) const;
	GLint getUniformLocation(const char* name) const;

	GLuint shaderProgramId = 0;

	glm::mat4 cameraView{ 1.0f };
	glm::mat4 cameraProjection{ 1.0f };
	glm::vec3 cameraPosition{ 0.0f };
};
