#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

class Application;

class Window
{
public:
	Application* application;
	GLFWwindow* window;
	Window(Application* app);
	void ImGuiRefresh(glm::vec3& rotation);
	void SetMouseCallback(void(*func)(GLFWwindow*, double, double));
	~Window();
};

