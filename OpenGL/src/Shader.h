#pragma once
#include <string>
#include "glm/glm.hpp"

class Shader
{
private:
	std::string m_VertFilepath;
	std::string m_FragFilepath;


public:
	unsigned int m_RendererID;
	Shader(const std::string& VertFilePath, const std::string& FragFilePath);
	~Shader();

	void Bind() const;
	void Unbind() const;


	void SetUniform2fv(const std::string& name, int count, float* data);

	void SetUniform2iv(const std::string& name, int count, int* data);

	// Uniforms
	void SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3);
	void SetUniform3f(const std::string& name, float v0, float v1, float v2);
	void SetUniformMat4f(const std::string& name, glm::mat4  matrix);

	std::string ParseShader(const std::string& filepath);
	unsigned int CompileShader(unsigned int type, const std::string& source);
	unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader);

	int GetUniformLocation(const std::string& name);
};
