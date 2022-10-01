#include <GL\glew.h>
#include "InstancedVB.h"


InstancedVB::InstancedVB(const void* data, unsigned int size,unsigned int instanceCount, int mode)
{
	m_InstanceCount = instanceCount;
	m_Count = size / sizeof(float);
	glGenBuffers(1, &m_RendererID);
	glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
	glBufferData(GL_ARRAY_BUFFER, size, data, mode);
}

InstancedVB::InstancedVB()
{
	glGenBuffers(1, &m_RendererID);
}

void InstancedVB::CreateData(const void* data, unsigned int size, unsigned int instanceCount, int mode) {
	m_InstanceCount = instanceCount;
	m_Count = size / sizeof(float);
	//glGenBuffers(1, &m_RendererID);
	glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
	glBufferData(GL_ARRAY_BUFFER, size, data, mode);
}

InstancedVB::~InstancedVB()
{
	glDeleteBuffers(1, &m_RendererID);

}

void InstancedVB::Bind() const
{
	glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);

}

void InstancedVB::Unbind() const
{
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}
