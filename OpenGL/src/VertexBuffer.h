#pragma once
class VertexBuffer
{
private:
public:
	unsigned int m_RendererID;
	VertexBuffer(const void* data, unsigned int size,int mode);
	~VertexBuffer();
	
	void Bind() const;
	void Unbind() const;

};

