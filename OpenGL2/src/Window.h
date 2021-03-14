#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

class Window
{
public:
	GLFWwindow* window;;
	Window();
	void ImGuiRefresh(glm::vec3& rotation);
	~Window();
};

