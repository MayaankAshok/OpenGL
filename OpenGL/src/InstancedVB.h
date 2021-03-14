#pragma once
class InstancedVB
{
private:
public:
	unsigned int m_RendererID;
	unsigned int m_Count;
	InstancedVB(const void* data, unsigned int size, int mode);
	~InstancedVB();

	void Bind() const;
	void Unbind() const;
};

