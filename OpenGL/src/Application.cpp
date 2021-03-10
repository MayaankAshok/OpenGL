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
#include "Window.h"

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

	double lastX, lastY;
	float yaw=-90.0f, pitch;

};

void processInput(GLFWwindow* window, Camera& camera) {

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
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera.position += glm::normalize(glm::cross(camera.camFront, camera.camUp)) * cameraSpeed;
	if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS)
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS)
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

};

class MainGame {
public:
	Window window{};
	Block block{};
	Camera camera{};

	bool firstMouse = true;
	
	MainGame() {

		int* indices = &block.indices[0];

		glm::mat4 proj = glm::perspective(3.14159f * 0.5f, 1.777f, 0.1f, 100.0f);
		//glm::mat4 proj = glm::ortho(-.96f, .96f, -.54f, .54f,-1.0f, 1.0f);
		glm::mat4 view = glm::lookAt(camera.position, camera.target, camera.up);
		glm::vec3 rotation{ 0,0,0 };
		glm::mat4 model;

		glm::mat4 modelX;
		glm::mat4 modelY;
		glm::mat4 modelZ;
		glm::mat4 modelScale;
		glm::mat4 modelTranslate;

		print(block.vertices.data());
		print(&block.vertices[0]);
		float* vertices = &block.vertices[0].position.x;

		// Create Renderer
		Renderer renderer;

		// Vertex Buffers
		VertexBuffer vb(vertices, 8 * 6 * sizeof(float), GL_STATIC_DRAW);

		// Vertex Array Setup
		VertexArray va;
		VertexBufferLayout layout;
		layout.Push<float>(3);
		layout.Push<float>(3);
		va.AddBuffer(vb, layout);

		// Index buffer must come after Vertex array setup
		IndexBuffer ib(indices, 12 * 3);

		// Shaders
		Shader shader("res/shader/vertex.shader",
			"res/shader/fragment.shader");
		shader.Bind();
		float r = 0.6f;
		float increment = 0.05f;

		// Unbind All Buffers
		shader.Unbind();
		va.Unbind();
		vb.Unbind();
		ib.Unbind();

		//Main Loop
		while (!glfwWindowShouldClose(window.window))
		{
			renderer.Clear();
			window.ImGuiRefresh(rotation);

			modelX = glm::rotate(glm::mat4(1.0f), rotation.x, glm::vec3{ 1,0,0 });
			modelY = glm::rotate(glm::mat4(1.0f), rotation.y, glm::vec3{ 0,1,0 });
			modelZ = glm::rotate(glm::mat4(1.0f), rotation.z, glm::vec3{ 0,0,1 });
			modelScale = glm::scale(glm::mat4(1.0f), glm::vec3{ 0.2,0.2,0.2 });
			modelTranslate = glm::translate(glm::mat4(1.0f), glm::vec3{ 0,0,-4 });

			view = glm::lookAt(camera.position, camera.position + camera.camFront, camera.up);

			model = modelTranslate * modelX * modelY * modelZ * modelScale;

			glm::mat4 mvp = proj * view * modelX * modelZ * modelY * modelScale;


			shader.Bind();
			shader.SetUniform3f("u_Color", r, 0.2f, 0.8f);
			shader.SetUniformMat4f("u_model", model);
			shader.SetUniformMat4f("u_view", view);
			shader.SetUniformMat4f("u_projection", proj);

			renderer.Draw(va, ib, shader);

			ImGui::Render();
			ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

			processInput(window.window, camera);

			/* Swap front and back buffers */
			glfwSwapBuffers(window.window);

			/* Poll for and process events */
			glfwPollEvents();
			handleMouse(window.window);
		}
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();

		glfwTerminate();
	}
	
	void handleMouse(GLFWwindow* window)
	{
		GLdouble xpos, ypos;
		glfwGetCursorPos(window, &xpos, &ypos);

		if (firstMouse)
		{

			camera.lastX = xpos;
			camera.lastY = ypos;
			firstMouse = false;
		}

		float xoffset = xpos - camera.lastX;
		float yoffset = camera.lastY - ypos; // reversed since y-coordinates go from bottom to top
		camera.lastX = xpos;
		camera.lastY = ypos;

		float sensitivity = 0.1f; // change this value to your liking
		xoffset *= sensitivity;
		yoffset *= sensitivity;

		camera.yaw += xoffset;
		camera.pitch += yoffset;

		// make sure that when pitch is out of bounds, screen doesn't get flipped
		if (camera.pitch > 89.0f)
			camera.pitch = 89.0f;
		if (camera.pitch < -89.0f)
			camera.pitch = -89.0f;
		print(camera.pitch << "   " << camera.yaw);

		glm::vec3 front;
		front.x = cos(glm::radians(camera.yaw)) * cos(glm::radians(camera.pitch));
		front.y = sin(glm::radians(camera.pitch));
		front.z = sin(glm::radians(camera.yaw)) * cos(glm::radians(camera.pitch));
		camera.camFront = glm::normalize(front);
	}
};

int main(void)
{
	MainGame maingame{};
	print("done");
	return 0;
}

