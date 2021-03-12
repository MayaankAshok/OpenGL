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

	std::cout << ib.GetCount();

	glDrawElements(GL_TRIANGLES, ib.GetCount(), GL_UNSIGNED_INT, nullptr);
}
void Renderer::Draw(VertexArray& va, Shader& shader)
{
	va.Bind();

	glDrawArrays(GL_TRIANGLES, 0,36);
}