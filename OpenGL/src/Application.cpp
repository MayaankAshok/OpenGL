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
	Vec3 normal;
};

float getMagn(Vec3 vec) {
	return sqrtf(vec.x * vec.x + vec.y * vec.y + vec.z * vec.z);
}

Vec3 normalize(Vec3 vec) {
	float magn = getMagn(vec);
	return { vec.x / magn, vec.y / magn,vec.z / magn };
}

struct Block
{
	std::vector<Vertex> vertices = {Vertex{-1.0f,-1.0f,1.0f, -1.0f,-1.0f,1.0f},
									Vertex{1.0f,-1.0f,1.0f, 1.0f,-1.0f, 1.0f},
									Vertex{1.0f,1.0f,1.0f, 1.0f,1.0f,1.0f},
									Vertex{-1.0f,1.0f,1.0f, -1.0f,1.0f,1.0f},
									Vertex{-1.0f,-1.0f,-1.0f, -1.0f,-1.0f,-1.0f},
									Vertex{1.0f,-1.0f,-1.0f, 1.0f,-1.0f,-1.0f } ,
									Vertex{1.0f,1.0f,-1.0f, 1.0f,1.0f,-1.0f},
									Vertex{-1.0f,1.0f,-1.0f, -1.0f,1.0f,-1.0f} };
	
	std::vector<int> indices = {
		0,1,2,
		2,3,0,

		1,5,6,
		6,2,1,
		
		7,6,5,
		5,4,7,
		
		4,0,3,
		3,7,4,
		
		4,5,1,
		1,0,4,
		
		3,2,6,
		6,7,3,
	};

	Block() {
		for (auto vertex : vertices) {
			vertex.normal = normalize(vertex.normal);
			//print(getMagn(vertex.normal));
		}

	}


};

struct Camera
{
	glm::vec3 position = { 0,0,10 };
	glm::vec3 target = { 0.0f,0.0f,0.0f };
	glm::vec3 direction = glm::normalize(position-target);
	glm::vec3 up = { 0,1,0 };
	glm::vec3 camRight  = glm::normalize(glm::cross(up,direction));
	glm::vec3 camUp = glm::normalize(glm::cross( direction,camRight));
	glm::vec3 camFront = { 0,0,-1 };

};

void processInput(GLFWwindow* window,Camera &camera) {
	
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
	float cameraSpeed = 0.05f;
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera.position += cameraSpeed * camera.camFront;
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera.position -= cameraSpeed * camera.camFront;
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera.position -= glm::normalize(glm::cross(camera.camFront, camera.camUp)) * cameraSpeed;
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera.position += glm::normalize(glm::cross(camera.camFront, camera.camUp)) * cameraSpeed;
}
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
	glEnable(GL_DEPTH_TEST);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	
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

	Block block = Block();
	Camera camera = Camera();
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


	//unsigned int indices[] = {
		//0,1,2,
		//2,3,0
	//};

	int* indices = &block.indices[0];

	glm::mat4 proj = glm::perspective(3.14159f*0.05f, 1.777f, 0.1f, 100.0f);
	//glm::mat4 proj = glm::ortho(-.96f, .96f, -.54f, .54f,-1.0f, 1.0f);
	glm::mat4 view = glm::lookAt(camera.position, camera.target, camera.up);
	glm::vec3 rotation{ 0,0,0 };
	glm::mat4 model;

	//float vertices[] = {
	//-0.1f, -0.2f, 0.0f,
	//0.1f, -0.2f, 0.0f,
	//0.1f, 0.2f, 0.0f,
	//-0.1f, 0.2f, 0.0f
	//};

	print(block.vertices.data());
	print(&block.vertices[0]);
	float* vertices = &block.vertices[0].position.x;

	// Create Renderer
	Renderer renderer;

	// Vertex Buffers
	VertexBuffer vb(vertices, 8 * 6 * sizeof(float),GL_STATIC_DRAW);

	// Vertex Array Setup
	VertexArray va;
	VertexBufferLayout layout;
	layout.Push<float>(3);
	layout.Push<float>(3);
	va.AddBuffer(vb, layout);

	// Index buffer must come after Vertex array setup
	IndexBuffer ib(indices, 12*3);

	// Shaders
	Shader shader("res/shader/vertex.shader",
		"res/shader/fragment.shader");
	shader.Bind();
	float r = 0.6f;
	float increment = 0.05f;

	//shader.SetUniform4f("u_Color", r, 0.5f, 0.5f, 1.0f);


	// Unbind All Buffers
	shader.Unbind();
	va.Unbind();
	vb.Unbind();
	ib.Unbind();
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

			ImGui::SliderFloat3("translation", &rotation.x, -1.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0fton"))                            // Buttons return true when clicked (most widgets return true when edited/activated)

			ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
			ImGui::End();
		}

		/* Render here */

		renderer.Clear();

		//if (r > 1 || r < 0) {
			//increment *= -1;
		//}

		//r += increment;

		glm::mat4 modelX = glm::rotate(glm::mat4(1.0f), rotation.x, glm::vec3{ 1,0,0 });
		glm::mat4 modelY = glm::rotate(glm::mat4(1.0f), rotation.y, glm::vec3{ 0,1,0 });
		glm::mat4 modelZ = glm::rotate(glm::mat4(1.0f), rotation.z, glm::vec3{ 0,0,1 });
		glm::mat4 modelScale = glm::scale(glm::mat4(1.0f), glm::vec3{ 0.2,0.2,0.2 });
		glm::mat4 modelTranslate = glm::translate(glm::mat4(1.0f), glm::vec3{ 0,0,-4 });

		view = glm::lookAt(camera.position, camera.position+camera.camFront, camera.up);


		model = modelTranslate* modelX * modelY * modelZ* modelScale;

		glm::mat4 mvp = proj * view * modelX * modelZ * modelY * modelScale;

		
		shader.Bind();
		shader.SetUniform3f("u_Color", r, 0.2f, 0.8f);
		shader.SetUniformMat4f("u_model",model);
		shader.SetUniformMat4f("u_view", view);
		shader.SetUniformMat4f("u_projection", proj);


		renderer.Draw(va, ib, shader);

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		processInput(window,camera);

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

