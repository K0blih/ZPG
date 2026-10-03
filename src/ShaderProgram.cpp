#include "ShaderProgram.h"
#include "Shader.h"

#include <stdexcept>
#include <string>

void ShaderProgram::link(const Shader& vertex, const Shader& fragment)
{
	reset();

	shaderProgramId = glCreateProgram();
	if (shaderProgramId == 0)
	{
		throw std::runtime_error("Unable to create shader program");
	}

	vertex.attachTo(shaderProgramId);
	fragment.attachTo(shaderProgramId);

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

	vertex.detachFrom(shaderProgramId);
	fragment.detachFrom(shaderProgramId);
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

GLint ShaderProgram::getUniformLocation(const char* name) const
{
	if (shaderProgramId == 0)
	{
		throw std::runtime_error("Shader program is not linked");
	}

	GLint location = glGetUniformLocation(shaderProgramId, name);

	if (location == -1)
	{
		throw std::runtime_error(std::string("Uniform is missing or inactive: ") + name);
	}

	return location;
}

void ShaderProgram::setUniform(const char* name, float value) const
{
	GLint location = getUniformLocation(name);
	glUniform1f(location, value);
}

void ShaderProgram::setUniform(const char* name, const glm::vec3& value) const
{
	GLint location = getUniformLocation(name);
	glUniform3f(location, value.x, value.y, value.z);
}

ShaderProgram::~ShaderProgram()
{
	reset();
}