#include "VertexBuffer.h"
#include <GL\glew.h>

VertexBuffer::VertexBuffer(const void* data, unsigned int size,int mode)
{
	m_Count = size / sizeof(float);
	glGenBuffers(1, &m_RendererID);
	glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
	glBufferData(GL_ARRAY_BUFFER, size, data, mode);
}
VertexBuffer::VertexBuffer()
{
	glGenBuffers(1, &m_RendererID);
}
void VertexBuffer::CreateData(const void* data, unsigned int size, int mode)
{
	m_Count = size / sizeof(float);
	glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
	glBufferData(GL_ARRAY_BUFFER, size, data, mode);
}

VertexBuffer::~VertexBuffer()
{
	glDeleteBuffers(1, &m_RendererID);
}

void VertexBuffer::Bind() const
{
	glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
}

void VertexBuffer::Unbind() const
{
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}