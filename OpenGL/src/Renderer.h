#pragma once
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "Shader.h"
class Renderer
{
private:
public:
	void Clear()const;
	void Draw(VertexArray& va, IndexBuffer& ib, Shader& shader);
	void Draw(VertexArray& va, Shader& shader);
	void DrawInstanced(VertexArray& va, Shader& shader, int NumTriangles, int numInstances);
};
