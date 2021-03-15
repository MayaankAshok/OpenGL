#include "Renderer.h"
#include <iostream>

void Renderer::Clear() const
{
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
}

void Renderer::Draw(VertexArray& va, IndexBuffer& ib, Shader& shader)
{
	ib.Bind();
	va.Bind();
	shader.Bind();

	glDrawElements(GL_TRIANGLES, ib.GetCount(), GL_UNSIGNED_INT, nullptr);
}
void Renderer::Draw(VertexArray& va, Shader& shader)
{
	va.Bind();
	shader.Bind();

	glDrawArrays(GL_TRIANGLES, 0,36);
}
void Renderer::DrawInstanced(VertexArray& va, Shader& shader,int NumTriangles,int numInstances)
{
	va.Bind();
	shader.Bind();
	glDrawArraysInstanced(GL_TRIANGLES, 0, NumTriangles, numInstances); // 100 triangles of 6 vertices each
}