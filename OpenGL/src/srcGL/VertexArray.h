#pragma once
#include "VertexBuffer.h"
#include "VertexBufferLayout.h"
#include "InstancedVB.h"
class VertexArray
{
private:
	unsigned int m_RendererID;

public:
	//unsigned int m_VertexCount;

	
	VertexArray();
	~VertexArray();
	
	void AddBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout);
	void AddBuffer(const VertexBuffer& vb, const InstancedVB& ivbBlock, const VertexBufferLayout& layout);
	void Bind() const;
	void Unbind ()const;
};









