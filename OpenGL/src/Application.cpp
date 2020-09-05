#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "ImGui/imgui.h"
#include "ImGui/imgui_impl_glfw.h"
#include "ImGui/imgui_impl_opengl3.h"
#include "ImGui/imgui_internal.h"

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <cmath>
#include <array>

#include "Renderer.h"
#include "IndexBuffer.h"
#include "VertexBuffer.h"
#include "VertexArray.h"
#include "VertexBufferLayout.h"
#include "Shader.h"

#define print(x) std::cout<<x<<std::endl

std::string ParseShader(const std::string& filepath) {
	std::ifstream stream(filepath);
	std::stringstream ss;
	std::string line;

	while (getline(stream, line)) {
		ss << line << "\n";
	}
	return ss.str();
}

unsigned int CompileShader(unsigned int type, const std::string& source) {
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

unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader)
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

struct Vec3
{
	float x, y, z;
};
struct Vec2
{
	float x, y;

};
struct Vec4
{
	float x, y, z, w;
};

struct Vertex
{
	Vec3 position;
	Vec4 color;
};

std::array<Vertex, 4> CreateQuad(float x, float y, float w, float h, Vec4 color) {

	Vertex v1;
	v1.position = { x,y,0 };
	v1.color = color;

	Vertex v2;
	v2.position = { x + w,y,0 };
	v2.color = color;

	Vertex v3;
	v3.position = { x + w,y + h+100,0 };
	v3.color = color;

	Vertex v4;
	v4.position = { x,y + h,0 };
	v4.color = color;

	return { v1,v2,v3,v4 };

};

int main(void)
{
	GLFWwindow* window;

	/* Initialize the library */
	if (!glfwInit())
		return -1;

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	/* Create a windowed mode window and its OpenGL context */
	window = glfwCreateWindow(1920, 1080, "Hello World", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		return 0;
	}

	/* Make the window's context current */
	glfwMakeContextCurrent(window);
	if (glewInit() != GLEW_OK) {
		std::cout << "Error" << std::endl;
	}
	glfwSwapInterval(1);

	std::cout << glGetString(GL_VERSION) << std::endl;

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	ImGuiStyle& style = ImGui::GetStyle();


	ImGui::StyleColorsDark();
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init("#version 330");

	style.ScaleAllSizes(1.5f);
	bool show_demo_window = true;
	bool show_another_window = false;
	ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);



	// Create Data for vertices
	/*
	float n = 50.0f;

	std::vector<float> positions;
	std::vector<unsigned int> indices;

	positions.push_back(0.0f);
	positions.push_back(0.0f);
	for (float i = 0; i < n; i++) {
		positions.push_back(0.5*std::cos(6.28f * i/n));
		positions.push_back(0.5*std::sin(6.28f * i/n));
	}

	for (unsigned int i = 1; i < n; i++) {
		indices.push_back(0);
		indices.push_back(i);
		indices.push_back(i+1);
	}
	print(positions.data());
	indices.push_back(0);
	indices.push_back((unsigned int)n);
	indices.push_back(1);
	*/


	unsigned int indices[] = {
		0,1,2,
		2,3,0
	};

	glm::mat4 proj = glm::ortho(0.0f,1920.0f, 0.0f, 1080.0f, -1.0f, 1.0f);
	glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0));
	glm::vec3 translation{ 0,0,0 };

	float vertices[] = {
	100.0f, 100.0f, 0.0f, 0.0f,1.0f,1.0f,1.0f,
	200.0f, 100.0f, 0.0f, 1.0f,1.0f,1.0f,1.0f,
	200.0f, 200.0f, 0.0f, 1.0f,1.0f,0.0f,1.0f,
	100.0f, 200.0f, 0.0f, 1.0f,1.0f,1.0f,0.0f,

	};

	// Vertex Buffers
	unsigned int vb;
	glGenBuffers(1, &vb);
	glBindBuffer(GL_ARRAY_BUFFER, vb);
	glBufferData(GL_ARRAY_BUFFER,4* 7 * sizeof(float), nullptr, GL_DYNAMIC_DRAW);

	// Vertex Array Setup
	unsigned int va;
	glGenVertexArrays(1, &va);
	glBindVertexArray(va);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 7* sizeof(float),0);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (const void*)(3 * sizeof(float)));

	// Index buffer must come after Vertex array setup
	unsigned int ib;
	glGenBuffers(1, &ib);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,ib );
	glBufferData(GL_ELEMENT_ARRAY_BUFFER,6* sizeof( unsigned int), indices, GL_STATIC_DRAW);

	// Shaders
	std::string VertSource = ParseShader("res/shader/vertex.shader");
	std::string FragSource = ParseShader("res/shader/fragment.shader");

	unsigned shader = CreateShader(VertSource, FragSource);
	glUseProgram(shader);
	glm::mat4 model = glm::translate(glm::mat4(1.0f), translation);

	glm::mat4 mvp = proj * view * model;
	int location = glGetUniformLocation(shader, "u_mvp");
	glUniformMatrix4fv(location, 1, false, &mvp[0][0]);

	float t = 0;


	// Unbind All Buffers
	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window))
	{
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		{
			static float f = 0.0f;
			static int counter = 0;

			ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.
			ImGui::SetWindowFontScale(2);

			ImGui::SliderFloat3("translation", &translation.x, 0.0f, 1000.0f);            // Edit 1 float using a slider from 0.0f to 1.0fton"))                            // Buttons return true when clicked (most widgets return true when edited/activated)

			ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
			ImGui::End();
		}

		/* Render here */
		glClear(GL_COLOR_BUFFER_BIT);


		//glm::mat4 model = glm::translate(glm::mat4(1.0f), translation);
		//glm::mat4 mvp = proj * view * model;
		//int location =  glGetUniformLocation(shader, "u_mvp");
		//glUniformMatrix4fv(location, 1, false, &mvp[0][0]);

		float vertices[] = {
			100.0f, 100.0f, 0.0f, 0.0f,1.0f,1.0f,1.0f,
			200.0f, 100.0f, 0.0f, 1.0f,1.0f,1.0f,1.0f,
			200.0f, t, 0.0f, 1.0f,1.0f,0.0f,1.0f,
			100.0f, 200.0f, 0.0f, 1.0f,1.0f,1.0f,0.0f
		};
		t++;
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
		
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		/* Swap front and back buffers */
		glfwSwapBuffers(window);

		/* Poll for and process events */
		glfwPollEvents();
	}
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	glfwTerminate();
	return 0;
}