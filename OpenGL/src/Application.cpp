#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/noise.hpp"
#include "ImGui/imgui.h"
#include "ImGui/imgui_impl_glfw.h"
#include "ImGui/imgui_impl_opengl3.h"
#include "ImGui/imgui_internal.h"

#ifndef STB_IMAGE_IMPLEMENTATION

#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"

#endif // !STB_IMAGE_IMPLEMENTATION


#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <cmath>
#include <array>


#include "srcGL/Renderer.h"
#include "srcGL/IndexBuffer.h"
#include "srcGL/VertexBuffer.h"
#include "srcGL/VertexArray.h"
#include "srcGL/VertexBufferLayout.h"
#include "srcGL/InstancedVB.h"
#include "srcGL/Shader.h"
#include "srcGL/Window.h"
#include "srcGL/Texture.h"

#include "Structures.h"
#include "Block.h"
#include "Chunk.h"

#define print(x) std::cout<<x<<std::endl

class Application;


struct Camera
{
	glm::vec3 position = { 1,10,5 };
	glm::vec3 up = { 0,1,0 };
	//glm::vec3 target = { 0.0f,0.0f,0.0f };
	//glm::vec3 direction = glm::normalize(position-target);
	glm::vec3 camFront = { 0,-1,0 };
	glm::vec3 camRight  = glm::normalize(glm::cross(camFront,up));
	glm::vec3 camUp = glm::normalize(glm::cross(camRight,camFront));

	float lastX, lastY;
	float yaw=0.0f, pitch=-90.0f;

};

class Controller {
public:
	Application* app;
	Camera* camera;
};

class Application {
public:

	Window window{this};
	ChunkCluster chunks{};

	Camera camera{};
	Controller controller{this,&camera};
	glm::vec3 rotation{ 0,0,0 };
	bool firstMouse = true;
	bool toMove = true;

	Application() {
		glEnable(GL_DEPTH_TEST);

		//glEnable(GL_POLYGON_STIPPLE);
		//glEnable(GL_CULL_FACE);
		glfwSetWindowUserPointer(window.window, this);
		glEnable(GL_BLEND);
		//glBlendFuncSep arate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ZERO);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		//glBlendFunc(GL_ONE, GL_ONE);
		glfwSetInputMode(window.window , GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		//window.SetMouseCallback(MouseCallback);
		//chunk2._Create({ 0,0 });

		glm::mat4 proj = glm::perspective(3.14159f * 0.5f, 1.777f, 0.1f, 100.0f);
		glm::mat4 view = glm::lookAt(camera.position, camera.position+ camera.camFront, camera.up);
		glm::mat4 model;
		glm::mat4 modelX;
		glm::mat4 modelY;
		glm::mat4 modelZ;
		glm::mat4 modelScale;
		glm::mat4 mvp;

		// Create Renderer
		Renderer renderer;

		// Vertex Buffers Setup
		//float* vertices = &Block::vertices[0].position.x;
		//VertexBuffer vb(vertices, 6 *  sizeof(Vertex), GL_STATIC_DRAW);
		//chunk.updateIVB();
		//InstancedVB ivb = chunk.ivb;
 		//print(ivb.m_InstanceCount);
		//
		// Vertex Array Setup
		//VertexArray va;
		//VertexBufferLayout layout;
		//layout.Push<float>(3, false);
		//layout.Push<float>(2, false);
		//layout.Push<float>(3, true);
		//layout.Push<float>(2, true);
		//layout.Push<float>(2, true);
		//
		//va.AddBuffer(vb, ivb, layout);

		Texture texture1("res/textures/DefaultPack.png");
		Texture texture2("res/textures/test.png");


		//unsigned int texture;
		//glGenTextures(1, &texture);
		//glBindTexture(GL_TEXTURE_2D, texture);
		//
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		//
		//// set texture filtering parameters
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		//
		//// load image, create texture and generate mipmaps
		//stbi_set_flip_vertically_on_load(true);
		//int width, height, nrChannels;
		//unsigned char* data = stbi_load("res/textures/DefaultPack.png", &width, &height, &nrChannels,STBI_rgb_alpha);
		//if (data)
		//{
		//	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		//	glGenerateMipmap(GL_TEXTURE_2D);
		//}
		//else
		//{
		//	std::cout << "Failed to load texture" << std::endl;
		//}
		//stbi_image_free(data);


		// Shaders
		Shader shader("res/shader/vertex.shader",
			"res/shader/fragment.shader");
		shader.Bind();
		float r = 0.6f;
		float increment = 0.05f;


		// Unbind All Buffers
		//shader.Unbind();
		//ib.Unbind();
		//handleMouse(534, -114);
		//
		//unsigned int framebuffer;
		//glGenFramebuffers(1, &framebuffer);
		//glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
		//
		//unsigned int texColorBuffer;
		//glGenTextures(1, &texColorBuffer);
		//glBindTexture(GL_TEXTURE_2D, texColorBuffer);
		//glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 800, 600, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		//glBindTexture(GL_TEXTURE_2D, 0);
		//
		//// attach it to currently bound framebuffer object
		//glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texColorBuffer, 0);
		//
		//unsigned int rbo;
		//glGenRenderbuffers(1, &rbo);
		//glBindRenderbuffer(GL_RENDERBUFFER, rbo);
		//glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, 800, 600);
		//glBindRenderbuffer(GL_RENDERBUFFER, 0);
		//
		//glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);
		//
		//if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		//	std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
		//
		//glBindFramebuffer(GL_FRAMEBUFFER, 0);
		//Main Loop
		while (!glfwWindowShouldClose(window.window))
		{
			renderer.Clear();
			window.ImGuiRefresh(rotation);
			
			//Update Matrices
			{
				modelX = glm::rotate(glm::mat4(1.0f), rotation.x, glm::vec3{ 1,0,0 });
				modelY = glm::rotate(glm::mat4(1.0f), rotation.y, glm::vec3{ 0,1,0 });
				modelZ = glm::rotate(glm::mat4(1.0f), rotation.z, glm::vec3{ 0,0,1 });
				modelScale = glm::scale(glm::mat4(1.0f), glm::vec3{ 0.5,0.5,0.5 });

				view = glm::lookAt(camera.position, camera.position + camera.camFront, camera.up);

				//normalMat = modelX * modelY * modelZ;
				model = modelX * modelY * modelZ * modelScale;
				mvp = proj * view * modelX * modelZ * modelY * modelScale;
			}
			
			//Update Shader
			{
				shader.Bind();
				shader.SetUniformMat4f("u_model", model);
				shader.SetUniformMat4f("u_view", view);
				shader.SetUniformMat4f("u_projection", proj);

				shader.setTexture("Tex1", 0, texture1);
				shader.setTexture("Tex2", 1, texture2);
			}

			//Main Draw Call
			//renderer.DrawInstanced(va, shader, 6, ivb.m_InstanceCount);
			//chunk1.RenderChunk(renderer,shader);
			//chunk2.RenderChunk(renderer, shader);
			//Chunk chunk = chunks.ChunkList[0];
			chunks.RenderChunks(renderer, shader,camera.position);
			/*Chunk* chunk = &chunks.ChunkList[0];
			chunk->RenderChunk(renderer, shader);*/
			/*glEnable(GL_DEPTH_TEST);
			renderer.DrawInstanced(chunk->vaBlock, shader, 6, chunk->ivbBlock.m_InstanceCount);
			glDisable(GL_DEPTH_TEST);
			renderer.DrawInstanced(chunk->vaWater, shader, 6, chunk->ivbWater.m_InstanceCount);*/
			//print(camera.camFront.x<<" "<< camera.camFront.y << " " << camera.camFront.z );

			// Handle Input and refresh Screen
			{
				ImGui::Render();
				ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

				processInput(window.window, camera);

				/* Swap front and back buffers */
				glfwSwapBuffers(window.window);

				/* Poll for and process events */
				glfwPollEvents();
				handleMouse(window.window);
			}
		}
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();

		glfwTerminate();
	}
	inline static auto MouseCallback(
		GLFWwindow* window,
		double xpos,
		double ypos) -> void {
		Application* application = static_cast<Application*>(glfwGetWindowUserPointer(window));
		application->handleMouse2(xpos,ypos);
	}
	void handleMouse(GLFWwindow * window)
	{
		if (not toMove) { return; }
		GLdouble xposD, yposD;
		glfwGetCursorPos(window, &xposD, &yposD);
		float xpos = (float)xposD, ypos = (float)yposD;
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
		camera.camRight = glm::normalize(glm::cross(camera.camFront, camera.up));  // normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
		camera.camUp = glm::normalize(glm::cross(camera.camRight, camera.camFront));
		//print(camera.camFront.x << " " << camera.camFront.y << " " << camera.camFront.z );

	}
	void handleMouse2(double xposD, double yposD)
	{
		print(xposD << " " << yposD);
		if (not toMove) { return; }
		float xpos = (float)xposD, ypos = (float)yposD;
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
		camera.camRight = glm::normalize(glm::cross(camera.camFront, camera.up));  // normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
		camera.camUp = glm::normalize(glm::cross(camera.camRight, camera.camFront));
		//print(camera.camFront.x << " " << camera.camFront.y << " " << camera.camFront.z );

	}
	void processInput(GLFWwindow * window, Camera & camera) {

			if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
				glfwSetWindowShouldClose(window, true);

			float cameraSpeed = 0.05f;
			if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
				camera.position += cameraSpeed * camera.camFront;
			if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
				camera.position -= cameraSpeed * camera.camFront;
			if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
				camera.position -= camera.camRight * cameraSpeed;
			if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
				camera.position += camera.camRight * cameraSpeed;

			if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS)
				glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
			if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS)
				glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

			if (glfwGetKey(window, GLFW_KEY_M) == GLFW_PRESS)
				toMove = false;
			if (glfwGetKey(window, GLFW_KEY_N) == GLFW_PRESS)
				toMove = true;


			float rotationSpeed = 0.05f;

			//if (glfwGetKey(window, GLFW_KEY_U) == GLFW_PRESS)
			//	rotation.x += rotationSpeed;
			//if (glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS)
			//	rotation.y += rotationSpeed;
			//if (glfwGetKey(window, GLFW_KEY_O) == GLFW_PRESS)
			//	rotation.z += rotationSpeed;
			//if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS)
			//	rotation.x -= rotationSpeed;
			//if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS)
			//	rotation.y -= rotationSpeed;
			//if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS)
			//	rotation.z -= rotationSpeed;

			//float movementSpeed = 0.05f;
			//if (glfwGetKey(window, GLFW_KEY_N) == GLFW_PRESS)
			//	block.position.x += movementSpeed;
			//if (glfwGetKey(window, GLFW_KEY_M) == GLFW_PRESS)
			//	block.position.x -= movementSpeed;


		};
};

int main(void)
{
	Application maingame{};
	
	print("done");
	return 0;
}


