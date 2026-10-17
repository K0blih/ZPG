#include "ShaderProgram.h"
#include "Shader.h"

#include <GLFW/glfw3.h>

#include <stdexcept>
#include <string>
#include <ctime>

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

	setUniform("viewMatrix", cameraView);
	setUniform("projectionMatrix", cameraProjection);

	GLint location = glGetUniformLocation(shaderProgramId, "time");
	if (location != -1)
	{
		const std::time_t now = std::time(nullptr);
		std::tm local{};

		if (localtime_s(&local, &now) == 0)
		{
			const float seconds = static_cast<float>((local.tm_hour % 12) * 3600 + local.tm_min * 60 + local.tm_sec);

			glUniform1f(location, seconds);
		}
	}
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

void ShaderProgram::setUniform(const char* name, const glm::mat4& value) const
{
	GLint location = getUniformLocation(name);
	glUniformMatrix4fv(location, 1, GL_FALSE, &value[0][0]);
}

void ShaderProgram::onCameraChanged(const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix)
{
	cameraView = viewMatrix;
	cameraProjection = projectionMatrix;
}

ShaderProgram::~ShaderProgram()
{
	reset();
}