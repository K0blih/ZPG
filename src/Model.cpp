#include "Model.h"

#include <stdexcept>

void Model::create(const float* vertices, std::size_t floatCount, VertexFormat format, GLenum drawingMode)
{
	GLsizei floatsPerVertex = 0;

	switch (format)
	{
		case VertexFormat::PositionNormal:
			floatsPerVertex = 6;
			break;
		case VertexFormat::PositionColor:
			floatsPerVertex = 6;
			break;
		case VertexFormat::PositionColorNormal:
			floatsPerVertex = 9;
			break;
		default:
			throw std::invalid_argument("Unsupported vertex format");
	}

	if (!vertices || floatCount == 0 || floatCount % floatsPerVertex != 0)
	{
		throw std::invalid_argument("Invalid model vertex data");
	}

	reset();

	// Vertex Array Object (VAO)
	glGenVertexArrays(1, &VAO);
	// Vertex buffer object (VBO)
	glGenBuffers(1, &VBO);

	if (VAO == 0 || VBO == 0)
	{
		reset();
		throw std::runtime_error("Unable to create model buffers");
	}

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	glBufferData(GL_ARRAY_BUFFER, floatCount * sizeof(float), vertices, GL_STATIC_DRAW);

	//enable vertex attributes
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);

	// index, number of components, data type, normalized, vertex stride, offset
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, floatsPerVertex * sizeof(float), (GLvoid*)0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, floatsPerVertex * sizeof(float), (GLvoid*)(3 * sizeof(float)));

	vertexCount = static_cast<GLsizei>(floatCount / floatsPerVertex);
	this->drawingMode = drawingMode;

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Model::draw() const
{
	if (vertexCount == 0)
	{
		return;
	}

	glBindVertexArray(VAO);
	glDrawArrays(drawingMode, 0, vertexCount);
}

void Model::reset()
{
	if (VAO != 0)
	{
		glDeleteVertexArrays(1, &VAO);
		VAO = 0;
	}

	if (VBO != 0)
	{
		glDeleteBuffers(1, &VBO);
		VBO = 0;
	}

	vertexCount = 0;
}

Model::~Model()
{
	reset();
}