#pragma once
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "Shader.h"
class Renderer
{
private:
public:
	void Clear()const;
	void Draw(VertexArray& vaBlock, IndexBuffer& ib, Shader& shader);
	void Draw(VertexArray& vaBlock, Shader& shader);
	void DrawInstanced(VertexArray& vaBlock, Shader& shader, int NumTriangles, int numInstances);
	void DrawInstancedWireFrame(VertexArray& vaBlock, Shader& shader, int NumTriangles, int numInstances);
};
