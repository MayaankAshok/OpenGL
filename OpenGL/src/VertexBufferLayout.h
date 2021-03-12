#pragma once
#include <vector>
#include "GL/glew.h"

struct VertexBufferElement
{
	unsigned int type;
	unsigned int count;
	unsigned char normalized;

	static unsigned int GetSize(unsigned int type) {
		switch (type) {
			case GL_FLOAT:return 4;


		}
		return 0;
	}
};

class VertexBufferLayout
{
private:
	std::vector<VertexBufferElement> m_Elements;
	unsigned int m_Stride;

public:
	VertexBufferLayout()
		:m_Stride(0)
	{};

	template<typename T>
	void Push(unsigned int count) {
		static_assert(false);
	}

	template<>
	void Push<float>(unsigned int count ) {
		m_Elements.push_back({ GL_FLOAT,count,GL_FALSE });
		m_Stride += count * sizeof(float);
	}
	template<>
	void Push<int>(unsigned int count) {
		m_Elements.push_back({ GL_INT,count,GL_FALSE });
		m_Stride += count * sizeof(int);
	}

	inline std::vector<VertexBufferElement> GetElements()const { return m_Elements; }
	inline unsigned int GetStride()const{ return m_Stride; }



};