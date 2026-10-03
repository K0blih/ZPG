#pragma once

#include <glad/gl.h>

class Shader {
public:
    Shader(GLenum shaderType, const char* filePath);
    ~Shader();

    Shader(const Shader&) = delete;
    Shader& operator=(const Shader&) = delete;

	void attachTo(GLuint programId) const;
	void detachFrom(GLuint programId) const;

private:
    GLuint shaderId = 0;
};