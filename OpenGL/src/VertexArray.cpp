#include "VertexArray.h"
#include "GL/glew.h"
#include <iostream>
VertexArray::VertexArray()
{
	glGenVertexArrays(1, &m_RendererID);
}

VertexArray::~VertexArray()
{
	glDeleteVertexArrays(1, &m_RendererID);
}

void VertexArray::AddBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout)
{
	Bind();
	vb.Bind();
	m_VertexCount = vb.m_Count;
	const auto& elements = layout.GetElements();
	unsigned int offset = 0;
	for (unsigned int i = 0; i < elements.size(); i++) {
		const auto& element = elements[i];
		glEnableVertexAttribArray(i);
		if (element.type == GL_FLOAT) {
			glVertexAttribPointer(i, element.count, element.type, element.normalized
				, layout.GetStride(), (const void*)offset);
		}
		else if (element.type == GL_INT) {
			glVertexAttribIPointer(i, element.count, element.type
				, layout.GetStride(), (const void*)offset);
		}
		offset += element.GetSize(element.type) * element.count;
	}
}
void VertexArray::AddBuffer(const VertexBuffer& vb,const InstancedVB& ivb, const VertexBufferLayout& layout)
{
	Bind();
	vb.Bind();
	m_VertexCount = vb.m_Count;
	const auto& elements = layout.GetElements();
	unsigned int offset = 0;
	for (unsigned int i = 0; i < elements.size(); i++) {
		const auto& element = elements[i];
		glEnableVertexAttribArray(i);
		if (element.isInstanced == true) {
			ivb.Bind();
			glVertexAttribPointer(i, element.count, element.type, element.normalized
				, element.count*sizeof(float), (const void*)0);
			glVertexAttribDivisor(i, 1);
			vb.Bind();
		}
		if (element.type == GL_FLOAT) {
			glVertexAttribPointer(i, element.count, element.type, element.normalized
				, layout.GetStride(), (const void*)offset);
		}
		else if (element.type == GL_INT) {
			glVertexAttribIPointer(i, element.count, element.type
				, layout.GetStride(), (const void*)offset);
		}
		
		offset += element.GetSize(element.type) * element.count;
	}
}

void VertexArray::Bind() const
{
	glBindVertexArray(m_RendererID);
}

void VertexArray::Unbind() const
{
	glBindVertexArray(0);
}