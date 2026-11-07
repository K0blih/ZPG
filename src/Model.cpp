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
	case VertexFormat::PositionNormalColor:
		floatsPerVertex = 9;
		break;
	case VertexFormat::PositionNormalUv:
		floatsPerVertex = 8;
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

	GLsizei stride = static_cast<GLsizei>(floatsPerVertex * sizeof(float));

	//enable vertex attributes
	glEnableVertexAttribArray(0);
	// index, number of components, data type, normalized, vertex stride, offset
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (GLvoid*)0);

	if (format != VertexFormat::PositionNormal && format != VertexFormat::PositionNormalUv)
	{
		GLsizei colorOffset = format == VertexFormat::PositionNormalColor ? 6 : 3;

		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, stride, (GLvoid*)(colorOffset * sizeof(float)));
	}

	if (format != VertexFormat::PositionColor)
	{
		GLsizei normalOffset = format == VertexFormat::PositionColorNormal ? 6 : 3;

		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, stride, (GLvoid*)(normalOffset * sizeof(float)));
	}

	if (format == VertexFormat::PositionNormalUv)
	{
		GLsizei uvOffset = 6;
		glEnableVertexAttribArray(3);
		glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, stride, (GLvoid*)(uvOffset * sizeof(float)));
	}

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