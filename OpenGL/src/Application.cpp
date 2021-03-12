#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "ImGui/imgui.h"
#include "ImGui/imgui_impl_glfw.h"
#include "ImGui/imgui_impl_opengl3.h"
#include "ImGui/imgui_internal.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"

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
struct Vec3i
{
	int x, y, z;
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
	Vec2 TexCoords;
	Vec2 faceIndex;
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
	static std::vector<Vertex> vertices;
	//static std::vector<int> indices;
	Vec3 position;

	Block(float x, float y, float z) {
		position={ x,y,z };
	}


};
//std::vector<Vertex> Block::vertices = {
//			//position			//normal				// texcoord	//face
//	Vertex{-1.0f, -1.0f,  1.0f,	 0.0f,  0.0f,  1.0f,	0.0f, 0.0f, 0.0f,0.0f },//0  Front
//	Vertex{ 1.0f, -1.0f,  1.0f,	 0.0f,  0.0f,  1.0f,	1.0f, 0.0f,	0.0f,0.0f },//1,
//	Vertex{ 1.0f,  1.0f,  1.0f,	 0.0f,  0.0f,  1.0f,	1.0f, 1.0f,	0.0f,0.0f },//2,
//	Vertex{-1.0f, -1.0f,  1.0f,	 0.0f,  0.0f,  1.0f,	0.0f, 0.0f,	0.0f,0.0f },//0 
//	Vertex{ 1.0f,  1.0f,  1.0f,	 0.0f,  0.0f,  1.0f,	1.0f, 1.0f,	0.0f,0.0f },//2,
//	Vertex{-1.0f,  1.0f,  1.0f,	 0.0f,  0.0f,  1.0f,	0.0f, 1.0f,	0.0f,0.0f },//3,
//
//	Vertex{ 1.0f, -1.0f,  1.0f,	 1.0f,  0.0f,  0.0f,	0.0f, 0.0f,	1.0f,0.0f },//1, right
//	Vertex{ 1.0f, -1.0f, -1.0f,	 1.0f,  0.0f,  0.0f,	1.0f, 0.0f,	1.0f,0.0f },//5,
//	Vertex{ 1.0f,  1.0f, -1.0f,	 1.0f,  0.0f,  0.0f,	1.0f, 1.0f,	1.0f,0.0f },//6
//	Vertex{ 1.0f, -1.0f,  1.0f,	 1.0f,  0.0f,  0.0f,	0.0f, 0.0f,	1.0f,0.0f },//1,
//	Vertex{ 1.0f,  1.0f, -1.0f,	 1.0f,  0.0f,  0.0f,	1.0f, 1.0f,	1.0f,0.0f },//6
//	Vertex{ 1.0f,  1.0f,  1.0f,	 1.0f,  0.0f,  0.0f,	0.0f, 1.0f,	1.0f,0.0f },//2,
//
//	Vertex{ 1.0f, -1.0f, -1.0f,	 0.0f,  0.0f, -1.0f,	0.0f, 0.0f,	2.0f,0.0f },//5, back
//	Vertex{-1.0f, -1.0f, -1.0f,	 0.0f,  0.0f, -1.0f,	1.0f, 0.0f,	2.0f,0.0f },//4,
//	Vertex{-1.0f,  1.0f, -1.0f,	 0.0f,  0.0f, -1.0f,	1.0f, 1.0f,	2.0f,0.0f },//7
//	Vertex{ 1.0f, -1.0f, -1.0f,	 0.0f,  0.0f, -1.0f,	0.0f, 0.0f,	2.0f,0.0f },//5,
//	Vertex{-1.0f,  1.0f, -1.0f,	 0.0f,  0.0f, -1.0f,	1.0f, 1.0f,	2.0f,0.0f },//7
//	Vertex{ 1.0f,  1.0f, -1.0f,	 0.0f,  0.0f, -1.0f,	0.0f, 1.0f,	2.0f,0.0f },//6
//
//	Vertex{-1.0f,  1.0f,  1.0f,	-1.0f,  0.0f,  0.0f,	0.0f, 0.0f,	3.0f,0.0f },//3, left
//	Vertex{-1.0f,  1.0f, -1.0f,	-1.0f,  0.0f,  0.0f,	1.0f, 0.0f,	3.0f,0.0f },//7
//	Vertex{-1.0f, -1.0f, -1.0f,	-1.0f,  0.0f,  0.0f,	1.0f, 1.0f,	3.0f,0.0f },//4,
//	Vertex{-1.0f,  1.0f,  1.0f,	-1.0f,  0.0f,  0.0f,	0.0f, 0.0f,	3.0f,0.0f },//3,
//	Vertex{-1.0f, -1.0f, -1.0f,	-1.0f,  0.0f,  0.0f,	1.0f, 1.0f,	3.0f,0.0f },//4,
//	Vertex{-1.0f, -1.0f,  1.0f,	-1.0f,  0.0f,  0.0f,	0.0f, 1.0f,	3.0f,0.0f },//0 
//
//	Vertex{ 1.0f,  1.0f,  1.0f,	 0.0f,  1.0f,  0.0f,	0.0f, 0.0f,	4.0f,0.0f },//2, top
//	Vertex{ 1.0f,  1.0f, -1.0f,	 0.0f,  1.0f,  0.0f,	1.0f, 0.0f,	4.0f,0.0f },//6
//	Vertex{-1.0f,  1.0f, -1.0f,	 0.0f,  1.0f,  0.0f,	1.0f, 1.0f,	4.0f,0.0f },//7
//	Vertex{ 1.0f,  1.0f,  1.0f,	 0.0f,  1.0f,  0.0f,	0.0f, 0.0f,	4.0f,0.0f },//2,
//	Vertex{-1.0f,  1.0f, -1.0f,	 0.0f,  1.0f,  0.0f,	1.0f, 1.0f,	4.0f,0.0f },//7
//	Vertex{-1.0f,  1.0f,  1.0f,	 0.0f,  1.0f,  0.0f,	0.0f, 1.0f,	4.0f,0.0f },//3,
//	
//	Vertex{-1.0f, -1.0f, -1.0f,	 0.0f, -1.0f,  0.0f,	0.0f, 0.0f,	5.0f,0.0f },//4, bottom
//	Vertex{ 1.0f, -1.0f, -1.0f,	 0.0f, -1.0f,  0.0f,	1.0f, 0.0f,	5.0f,0.0f },//5,
//	Vertex{ 1.0f, -1.0f,  1.0f,	 0.0f, -1.0f,  0.0f,	1.0f, 1.0f,	5.0f,0.0f },//1,
//	Vertex{-1.0f, -1.0f, -1.0f,	 0.0f, -1.0f,  0.0f,	0.0f, 0.0f,	5.0f,0.0f },//4,
//	Vertex{ 1.0f, -1.0f,  1.0f,	 0.0f, -1.0f,  0.0f,	1.0f, 1.0f,	5.0f,0.0f },//1,
//	Vertex{-1.0f, -1.0f,  1.0f,	 0.0f, -1.0f,  0.0f,	0.0f, 1.0f,	5.0f,0.0f },//0 
//
//};
std::vector<Vertex> Block::vertices = {
			//position			//normal				// texcoord	//face
	Vertex{-1.0f, -1.0f,  1.0f,	 0.0f,  0.0f,  1.0f,	0.0f, 0.0f, 0.0f,0.0f },//0 
	Vertex{ 1.0f, -1.0f,  1.0f,	 0.0f,  0.0f,  1.0f,	1.0f, 0.0f,	0.0f,0.0f },//1,
	Vertex{ 1.0f,  1.0f,  1.0f,	 0.0f,  0.0f,  1.0f,	1.0f, 1.0f,	0.0f,0.0f },//2,
	Vertex{-1.0f, -1.0f,  1.0f,	 0.0f,  0.0f,  1.0f,	0.0f, 0.0f,	0.0f,0.0f },//0 
	Vertex{ 1.0f,  1.0f,  1.0f,	 0.0f,  0.0f,  1.0f,	1.0f, 1.0f,	0.0f,0.0f },//2,
	Vertex{-1.0f,  1.0f,  1.0f,	 0.0f,  0.0f,  1.0f,	0.0f, 1.0f,	0.0f,0.0f },//3,

	Vertex{ 1.0f, -1.0f,  1.0f,	 1.0f,  0.0f,  0.0f,	0.0f, 0.0f,	1.0f,0.0f },//1,
	Vertex{ 1.0f, -1.0f, -1.0f,	 1.0f,  0.0f,  0.0f,	1.0f, 0.0f,	1.0f,0.0f },//5,
	Vertex{ 1.0f,  1.0f, -1.0f,	 1.0f,  0.0f,  0.0f,	1.0f, 1.0f,	1.0f,0.0f },//6
	Vertex{ 1.0f, -1.0f,  1.0f,	 1.0f,  0.0f,  0.0f,	0.0f, 0.0f,	1.0f,0.0f },//1,
	Vertex{ 1.0f,  1.0f, -1.0f,	 1.0f,  0.0f,  0.0f,	1.0f, 1.0f,	1.0f,0.0f },//6
	Vertex{ 1.0f,  1.0f,  1.0f,	 1.0f,  0.0f,  0.0f,	0.0f, 1.0f,	1.0f,0.0f },//2,

	Vertex{ 1.0f, -1.0f, -1.0f,	 0.0f,  0.0f, -1.0f,	0.0f, 0.0f,	2.0f,0.0f },//5,
	Vertex{-1.0f, -1.0f, -1.0f,	 0.0f,  0.0f, -1.0f,	1.0f, 0.0f,	2.0f,0.0f },//4,
	Vertex{-1.0f,  1.0f, -1.0f,	 0.0f,  0.0f, -1.0f,	1.0f, 1.0f,	2.0f,0.0f },//7
	Vertex{ 1.0f, -1.0f, -1.0f,	 0.0f,  0.0f, -1.0f,	0.0f, 0.0f,	2.0f,0.0f },//5,
	Vertex{-1.0f,  1.0f, -1.0f,	 0.0f,  0.0f, -1.0f,	1.0f, 1.0f,	2.0f,0.0f },//7
	Vertex{ 1.0f,  1.0f, -1.0f,	 0.0f,  0.0f, -1.0f,	0.0f, 1.0f,	2.0f,0.0f },//6

	Vertex{-1.0f,  1.0f,  1.0f,	-1.0f,  0.0f,  0.0f,	1.0f, 1.0f,	3.0f,0.0f },//3,
	Vertex{-1.0f,  1.0f, -1.0f,	-1.0f,  0.0f,  0.0f,	0.0f, 1.0f,	3.0f,0.0f },//7
	Vertex{-1.0f, -1.0f, -1.0f,	-1.0f,  0.0f,  0.0f,	0.0f, 0.0f,	3.0f,0.0f },//4,
	Vertex{-1.0f,  1.0f,  1.0f,	-1.0f,  0.0f,  0.0f,	1.0f, 1.0f,	3.0f,0.0f },//3,
	Vertex{-1.0f, -1.0f, -1.0f,	-1.0f,  0.0f,  0.0f,	0.0f, 0.0f,	3.0f,0.0f },//4,
	Vertex{-1.0f, -1.0f,  1.0f,	-1.0f,  0.0f,  0.0f,	1.0f, 0.0f,	3.0f,0.0f },//0 

	Vertex{ 1.0f,  1.0f,  1.0f,	 0.0f,  1.0f,  0.0f,	1.0f, 0.0f,	4.0f,0.0f },//2,
	Vertex{ 1.0f,  1.0f, -1.0f,	 0.0f,  1.0f,  0.0f,	1.0f, 1.0f,	4.0f,0.0f },//6
	Vertex{-1.0f,  1.0f, -1.0f,	 0.0f,  1.0f,  0.0f,	0.0f, 1.0f,	4.0f,0.0f },//7
	Vertex{ 1.0f,  1.0f,  1.0f,	 0.0f,  1.0f,  0.0f,	1.0f, 0.0f,	4.0f,0.0f },//2,
	Vertex{-1.0f,  1.0f, -1.0f,	 0.0f,  1.0f,  0.0f,	0.0f, 1.0f,	4.0f,0.0f },//7
	Vertex{-1.0f,  1.0f,  1.0f,	 0.0f,  1.0f,  0.0f,	0.0f, 0.0f,	4.0f,0.0f },//3,

	Vertex{-1.0f, -1.0f, -1.0f,	 0.0f, -1.0f,  0.0f,	0.0f, 0.0f,	5.0f,0.0f },//4,
	Vertex{ 1.0f, -1.0f, -1.0f,	 0.0f, -1.0f,  0.0f,	1.0f, 0.0f,	5.0f,0.0f },//5,
	Vertex{ 1.0f, -1.0f,  1.0f,	 0.0f, -1.0f,  0.0f,	1.0f, 1.0f,	5.0f,0.0f },//1,
	Vertex{-1.0f, -1.0f, -1.0f,	 0.0f, -1.0f,  0.0f,	0.0f, 0.0f,	5.0f,0.0f },//4,
	Vertex{ 1.0f, -1.0f,  1.0f,	 0.0f, -1.0f,  0.0f,	1.0f, 1.0f,	5.0f,0.0f },//1,
	Vertex{-1.0f, -1.0f,  1.0f,	 0.0f, -1.0f,  0.0f,	0.0f, 1.0f,	5.0f,0.0f },//0 



};

struct Camera
{
	glm::vec3 position = { 0,0,5 };
	glm::vec3 target = { 0.0f,0.0f,0.0f };
	glm::vec3 direction = glm::normalize(position-target);
	glm::vec3 up = { 0,1,0 };
	glm::vec3 camRight  = glm::normalize(glm::cross(up,direction));
	glm::vec3 camUp = glm::normalize(glm::cross( direction,camRight));
	glm::vec3 camFront = { 0,0,-1 };

	double lastX, lastY;
	float yaw=-90.0f, pitch;

};



class MainGame {
public:
	Window window{};
	Block block{5,0,0};
	Camera camera{};
	glm::vec3 rotation{ 0,0,0 };

	bool firstMouse = true;
	
	MainGame() {

		//int* indices = &block.indices[0];

		glm::mat4 proj = glm::perspective(3.14159f * 0.5f, 1.777f, 0.1f, 100.0f);
		//glm::mat4 proj = glm::ortho(-.96f, .96f, -.54f, .54f,-1.0f, 1.0f);
		glm::mat4 view = glm::lookAt(camera.position, camera.target, camera.up);
		
		glm::mat4 model;

		glm::mat4 modelX;
		glm::mat4 modelY;
		glm::mat4 modelZ;
		glm::mat4 modelScale;
		glm::mat4 modelTranslate;
		glm::mat4 normalMat;


		//print(block.vertices.data());
		//print(&block.vertices[0]);
		float* vertices = &block.vertices[0].position.x;

		// Create Renderer
		Renderer renderer;

		// Vertex Buffers
		VertexBuffer vb(vertices, 124 * 10 * sizeof(float), GL_STATIC_DRAW);

		// Vertex Array Setup
		VertexArray va;
		VertexBufferLayout layout;
		layout.Push<float>(3);
		layout.Push<float>(3);
		layout.Push<float>(2);
		layout.Push<float>(2);


		va.AddBuffer(vb, layout);

		// Index buffer must come after Vertex array setup
		//IndexBuffer ib(indices, 12 * 3);

		unsigned int texture;
		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

		// set texture filtering parameters
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		// load image, create texture and generate mipmaps
		stbi_set_flip_vertically_on_load(true);
		int width, height, nrChannels;
		unsigned char* data = stbi_load("res/textures/DefaultPack.png", &width, &height, &nrChannels,STBI_rgb_alpha);
		if (data)
		{
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
			glGenerateMipmap(GL_TEXTURE_2D);
		}
		else
		{
			std::cout << "Failed to load texture" << std::endl;
		}
		stbi_image_free(data);


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
		//ib.Unbind();

		//Main Loop
		while (!glfwWindowShouldClose(window.window))
		{
			renderer.Clear();
			window.ImGuiRefresh(rotation);

			modelX = glm::rotate(glm::mat4(1.0f), rotation.x, glm::vec3{ 1,0,0 });
			modelY = glm::rotate(glm::mat4(1.0f), rotation.y, glm::vec3{ 0,1,0 });
			modelZ = glm::rotate(glm::mat4(1.0f), rotation.z, glm::vec3{ 0,0,1 });
			modelScale = glm::scale(glm::mat4(1.0f), glm::vec3{ 0.2,0.2,0.2 });
			modelTranslate = glm::translate(glm::mat4(1.0f), glm::vec3{ block.position.x,block.position.y,block.position.z });

			view = glm::lookAt(camera.position, camera.position + camera.camFront, camera.up);

			normalMat = modelX * modelY * modelZ;
			model = modelTranslate * modelX * modelY * modelZ * modelScale;

			glm::mat4 mvp = proj * view * modelX * modelZ * modelY * modelScale;


			shader.Bind();
			shader.SetUniform3f("u_Color", r, 0.2f, 0.8f);
			shader.SetUniformMat4f("u_model", model);
			shader.SetUniformMat4f("u_view", view);
			shader.SetUniformMat4f("u_projection", proj);
			shader.SetUniformMat4f("u_normal", normalMat);

			renderer.Draw(va, shader);

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
		//print(camera.pitch << "   " << camera.yaw);

		glm::vec3 front;
		front.x = cos(glm::radians(camera.yaw)) * cos(glm::radians(camera.pitch));
		front.y = sin(glm::radians(camera.pitch));
		front.z = sin(glm::radians(camera.yaw)) * cos(glm::radians(camera.pitch));
		camera.camFront = glm::normalize(front);
	}
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
		
		if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS)
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS)
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		
		float rotationSpeed = 0.05f;

		if (glfwGetKey(window, GLFW_KEY_U) == GLFW_PRESS)
			rotation.x += rotationSpeed;
		if (glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS)
			rotation.y += rotationSpeed;
		if (glfwGetKey(window, GLFW_KEY_O) == GLFW_PRESS)
			rotation.z += rotationSpeed;
		if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS)
			rotation.x -= rotationSpeed;
		if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS)
			rotation.y -= rotationSpeed;
		if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS)
			rotation.z -= rotationSpeed;

		float movementSpeed = 0.05f;
		if (glfwGetKey(window, GLFW_KEY_N) == GLFW_PRESS)
			block.position.x += movementSpeed;
		if (glfwGetKey(window, GLFW_KEY_M) == GLFW_PRESS)
			block.position.x -= movementSpeed;


	};
};

int main(void)
{
	MainGame maingame{};
	print("done");
	return 0;
}

