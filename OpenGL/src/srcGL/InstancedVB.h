#pragma once
class InstancedVB
{
private:
public:
	unsigned int m_RendererID;
	unsigned int m_Count;
	unsigned int m_InstanceCount;
	InstancedVB(const void* data, unsigned int size,unsigned int instanceCount, int mode);
	InstancedVB();

	void CreateData(const void* data, unsigned int size, unsigned int instanceCount, int mode);

	~InstancedVB();

	void Bind() const;
	void Unbind() const;
};

