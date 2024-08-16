// This include:
#include "vertexarray.h"

// Local includes:

// Library includes:
#include <glew.h> 
#include <cassert>

VertexArray::VertexArray(const float* pVertexData, unsigned int numVertices,
	const unsigned int* pIndexData, unsigned int numIndicies):
	m_numVertices(numVertices),
	m_numIndices(numIndicies),
	m_glVertexBuffer(0),
	m_glIndexBuffer(0),
	m_glVertexArray(0)
{
	const int stride = 5 * sizeof(float); //XYZUV
	assert(pVertexData);

	glGenVertexArrays(1, &m_glVertexArray);
	glBindVertexArray(m_glVertexArray);

	glGenBuffers(1, &m_glVertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, m_glVertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, m_numVertices * stride, pVertexData, GL_STATIC_DRAW);

	if (pIndexData)
	{
		glGenBuffers(1, &m_glIndexBuffer);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_glIndexBuffer);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_numIndices * sizeof(unsigned int), pIndexData, GL_STATIC_DRAW);
	}
	// Layout: XYZ 
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, 0);

	// Layout: UV 
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, stride, reinterpret_cast<void*>(sizeof(float) * 3));
}
VertexArray::~VertexArray()
{
	glDeleteBuffers(1, &m_glVertexBuffer);
	glDeleteBuffers(1, &m_glIndexBuffer);
	glDeleteVertexArrays(1, &m_glVertexArray);
}
void VertexArray::SetActive()
{
	glBindVertexArray(m_glVertexArray);
}

unsigned int VertexArray::GetNumVertices() const
{
	return m_numVertices;
}

unsigned int VertexArray::GetNumIndicies() const
{
	return m_numIndices;
}