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
			case GL_INT:return 4;

		}
		return 0;
	}
};

class VertexBufferLayout
{
private:
	std::vector<VertexBufferElement> m_ElementsUI;
	std::vector<VertexBufferElement> m_ElementsI;

	unsigned int m_StrideUI;
	unsigned int m_StrideI;

public:
	VertexBufferLayout()
		:m_StrideUI(0),m_StrideI(0)
	{};

	template<typename T>
	void Push(unsigned int count,bool instanced = false) {
		static_assert(false);
	}

	template<>
	void Push<float>(unsigned int count, bool instanced ) {
		
		if (instanced == true) {
			m_ElementsI.push_back({ GL_FLOAT,count,GL_FALSE });
			m_StrideI += count * sizeof(float);

		}
		else {
			m_ElementsUI.push_back({ GL_FLOAT,count,GL_FALSE });
			m_StrideUI += count * sizeof(float);
		}
	}
	template<>
	void Push<int>(unsigned int count, bool instanced) {
		if (instanced == true) {
			m_ElementsI.push_back({ GL_FLOAT,count,GL_FALSE });
			m_StrideI += count * sizeof(float);

		}
		else {
			m_ElementsUI.push_back({ GL_FLOAT,count,GL_FALSE });
			m_StrideUI += count * sizeof(float);
		}
	}

	inline std::vector<VertexBufferElement> GetElementsI()const { return m_ElementsI; }
	inline std::vector<VertexBufferElement> GetElementsUI()const { return m_ElementsUI; }
	inline unsigned int GetStrideUI()const{ return m_StrideUI; }
	inline unsigned int GetStrideI()const { return m_StrideI; }



};