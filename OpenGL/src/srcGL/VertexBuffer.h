#pragma once
class VertexBuffer
{
private:
public:
	unsigned int m_RendererID;
	unsigned int m_Count;
	VertexBuffer(const void* data, unsigned int size,int mode);
	VertexBuffer();
	void CreateData(const void* data, unsigned int size, int mode);
	~VertexBuffer();
	
	void Bind() const;
	void Unbind() const;

};

