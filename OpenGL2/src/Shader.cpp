#include "Shader.h"
#include "GL/glew.h"
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

Shader::Shader(const std::string& VertFilePath, const std::string& FragFilePath)
	:m_VertFilepath(VertFilePath), m_FragFilepath(FragFilePath), m_RendererID(0)
{
	std::string VertSource = ParseShader(VertFilePath);
	std::string FragSource = ParseShader(FragFilePath);

	m_RendererID = CreateShader(VertSource, FragSource);

	//Bind();
}

Shader::~Shader()
{
}

void Shader::Bind() const
{
	glUseProgram(m_RendererID);
}

void Shader::Unbind() const
{
	glUseProgram(0);
}

void Shader::SetUniform2fv(const std::string& name, int count, float* data)
{
	glUniform2fv(GetUniformLocation(name),count,data);
}
void Shader::SetUniform2iv(const std::string& name, int count, int* data)
{
	glUniform2iv(GetUniformLocation(name), count, data);
}
void Shader::SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3)
{
	glUniform4f(GetUniformLocation(name), v0, v1, v2, v3);
}


void Shader::SetUniform3f(const std::string& name, float v0, float v1, float v2)
{
	glUniform3f(GetUniformLocation(name), v0, v1, v2);
}


void Shader::SetUniformMat4f(const std::string& name, glm::mat4 matrix)
{
	glUniformMatrix4fv(GetUniformLocation(name), 1, false, &matrix[0][0]);
}

int Shader::GetUniformLocation(const std::string& name)
{
	return glGetUniformLocation(m_RendererID, name.c_str());
}

std::string Shader::ParseShader(const std::string& filepath) {
	std::ifstream stream(filepath);
	std::stringstream ss;
	std::string line;

	while (getline(stream, line)) {
		ss << line << "\n";
	}
	return ss.str();
}

unsigned int Shader::CompileShader(unsigned int type, const std::string& source) {
	unsigned int  id = glCreateShader(type);

	const char* src = source.c_str();
	glShaderSource(id, 1, &src, nullptr);
	glCompileShader(id);

	int result;
	glGetShaderiv(id, GL_COMPILE_STATUS, &result);
	if (result == GL_FALSE) {
		int length;
		glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
		char* message = (char*)alloca(length * sizeof(char));
		glGetShaderInfoLog(id, length, &length, message);
		std::cout << "Failed" << (type == GL_VERTEX_SHADER ? "Vertex" : "Fragment") << std::endl;
		std::cout << message << std::endl;
		glDeleteShader(id);
		return 0;
	}
	return id;
}

unsigned int Shader::CreateShader(const std::string& vertexShader, const std::string& fragmentShader)
{
	unsigned int program = glCreateProgram();
	unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
	unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

	glAttachShader(program, vs);
	glAttachShader(program, fs);
	glLinkProgram(program);
	glValidateProgram(program);

	glDeleteShader(vs);
	glDeleteShader(fs);

	return program;
}