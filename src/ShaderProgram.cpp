#include "ShaderProgram.h"
#include "Shader.h"

#include <stdexcept>

void ShaderProgram::link(const Shader& vertex, const Shader& fragment)
{
	reset();

	shaderProgramId = glCreateProgram();
	if (shaderProgramId == 0)
	{
		throw std::runtime_error("Unable to create shader program");
	}

	glAttachShader(shaderProgramId, vertex.getShaderId());
	glAttachShader(shaderProgramId, fragment.getShaderId());
	glLinkProgram(shaderProgramId);

	GLint success = GL_FALSE;
	glGetProgramiv(shaderProgramId, GL_LINK_STATUS, &success);

	if (!success)
	{
		char infoLog[1024];
		glGetProgramInfoLog(shaderProgramId, sizeof(infoLog), nullptr, infoLog);

		reset();
		throw std::runtime_error("Shader program linking failed:\n" + std::string(infoLog));
	}

	glDetachShader(shaderProgramId, vertex.getShaderId());
	glDetachShader(shaderProgramId, fragment.getShaderId());
}

void ShaderProgram::use() const
{
	glUseProgram(shaderProgramId);
}

void ShaderProgram::reset()
{
	if (shaderProgramId != 0)
	{
		glDeleteProgram(shaderProgramId);
		shaderProgramId = 0;
	}
}

ShaderProgram::~ShaderProgram()
{
	reset();
}