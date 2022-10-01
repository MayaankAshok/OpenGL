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

void VertexArray::AddBuffer(const VertexBuffer& vb,const InstancedVB& ivbBlock, const VertexBufferLayout& layout)
{
	Bind();
	vb.Bind();
	//m_VertexCount = vb.m_Count;
	const auto& elementsUI = layout.GetElementsUI();
	const auto& elementsI = layout.GetElementsI();
	unsigned int offsetUI = 0;
	unsigned int offsetI = 0;
	unsigned int AttribIndex=0;
	unsigned int IndexI = 0;

	while  (AttribIndex< elementsUI.size()) {
		const auto& element = elementsUI[AttribIndex];
		glEnableVertexAttribArray(AttribIndex);
		if (element.type == GL_FLOAT) {
			glVertexAttribPointer(AttribIndex, element.count, element.type, element.normalized
				, layout.GetStrideUI(), (const void*)offsetUI);
		}
		else if (element.type == GL_INT) {
			glVertexAttribIPointer(AttribIndex, element.count, element.type
				, layout.GetStrideUI(), (const void*)offsetUI);
		}
		
		offsetUI += element.GetSize(element.type) * element.count;
		AttribIndex += 1;
	}
	ivbBlock.Bind();
	while (AttribIndex < elementsUI.size() + elementsI.size()) {
		const auto& element = elementsI[IndexI];
		glEnableVertexAttribArray(AttribIndex);
		if (element.type == GL_FLOAT) {
			glVertexAttribPointer(AttribIndex, element.count, element.type, element.normalized
				, layout.GetStrideI(), (const void*)offsetI);
		}
		else if (element.type == GL_INT) {
			glVertexAttribIPointer(AttribIndex, element.count, element.type
				, layout.GetStrideI(), (const void*)offsetI);
		}
		glVertexAttribDivisor(AttribIndex, 1);
		offsetI += element.GetSize(element.type) * element.count;

		AttribIndex += 1;
		IndexI += 1;
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