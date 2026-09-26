#pragma once

#include <glad/gl.h>
#include <cstddef>

class Model {
public:
	Model() = default;
	~Model();

	Model(const Model&) = delete;
	Model& operator=(const Model&) = delete;

	void create(const float* vertices, std::size_t floatCount, GLenum drawingMode = GL_TRIANGLES);

	void draw() const;
	void reset();

private:
	GLuint VBO = 0;
	GLuint VAO = 0;
	GLsizei vertexCount = 0;
	GLenum drawingMode = GL_TRIANGLES;
};