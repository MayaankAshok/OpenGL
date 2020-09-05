#pragma once
#include <string>
#include "glm/glm.hpp"

class Shader
{
private:
	std::string m_VertFilepath;
	std::string m_FragFilepath;

	unsigned int m_RendererID;

public:
	Shader(const std::string& VertFilePath, const std::string& FragFilePath);
	~Shader();

	void Bind() const;
	void Unbind() const;

	// Uniforms
	void SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3);
	void SetUniformMat4f(const std::string& name, glm::mat4  matrix);

	std::string ParseShader(const std::string& filepath);
	unsigned int CompileShader(unsigned int type, const std::string& source);
	unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader);

	int GetUniformLocation(const std::string& name);
};
