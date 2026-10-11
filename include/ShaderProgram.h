#pragma once

#include <glad/gl.h>
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>

class Shader;

class ShaderProgram {
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

private:
	GLint getUniformLocation(const char* name) const;

	GLuint shaderProgramId = 0;
};