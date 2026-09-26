#pragma once

#include <glad/gl.h>

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

	GLuint getShaderProgramId() const { return shaderProgramId; }

private:
	GLuint shaderProgramId = 0;
};