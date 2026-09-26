#pragma once

#include <glad/gl.h>

class Shader {
public:
    Shader(GLenum shaderType, const char* filePath);
    ~Shader();

    Shader(const Shader&) = delete;
    Shader& operator=(const Shader&) = delete;

    GLuint getShaderId() const { return shaderId; }

private:
    GLuint shaderId = 0;
};