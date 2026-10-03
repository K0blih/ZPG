#include "Shader.h"

#include <fstream>
#include <stdexcept>

Shader::Shader(GLenum shaderType, const char* filePath)
{
	// Creates an empty shader
	shaderId = glCreateShader(shaderType);

	if (shaderId == 0)
	{
		throw std::runtime_error("Unable to create shader");
	}

	//Loading the contents of a file into a variable
	std::ifstream file(filePath);
	if (!file.is_open())
	{
		glDeleteShader(shaderId);
		shaderId = 0;
		throw std::runtime_error("Unable to open shader file:\n" + std::string(filePath));
	}
	std::string shaderCode((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());

	// Set the shader source code
	const char* source = shaderCode.c_str();
	glShaderSource(shaderId, 1, &source, nullptr);

	// Compile the shader source code
	glCompileShader(shaderId);

	// Check specialization/compilation status
	GLint success = GL_FALSE;
	glGetShaderiv(shaderId, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		char infoLog[1024];
		glGetShaderInfoLog(shaderId, sizeof(infoLog), nullptr, infoLog);

		glDeleteShader(shaderId);
		shaderId = 0;

		throw std::runtime_error("Shader compilation failed:\n" + std::string(infoLog));
	}
}

Shader::~Shader()
{
	if (shaderId != 0)
	{
		glDeleteShader(shaderId);
	}
}

void Shader::attachTo(GLuint programId) const
{
	glAttachShader(programId, shaderId);
}

void Shader::detachFrom(GLuint programId) const
{
	glDetachShader(programId, shaderId);
}
