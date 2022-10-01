#include "Window.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "ImGui/imgui.h"
#include "ImGui/imgui_impl_glfw.h"
#include "ImGui/imgui_impl_opengl3.h"
#include "ImGui/imgui_internal.h"

#define print(x) std::cout<<x<<std::endl



Window::Window(Application* app)
{
	application = app;
	//GLFWwindow* window;

	/* Initialize the library */
	if (!glfwInit())
	{
		print("GLFW INIT Failed");
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	/* Create a windowed mode window and its OpenGL context */
	window = glfwCreateWindow(1920, 1080, "Hello World", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
	}

	

	/* Make the window's context current */
	glfwMakeContextCurrent(window);
	if (glewInit() != GLEW_OK) {
		std::cout << "Error" << std::endl;
	}
	glfwSwapInterval(1);

	std::cout << glGetString(GL_VERSION) << std::endl;
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	glProvokingVertex(GL_FIRST_VERTEX_CONVENTION);

	//Set up ImGUI
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	ImGuiStyle& style = ImGui::GetStyle();


	ImGui::StyleColorsDark();
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init("#version 330");

	style.ScaleAllSizes(0.5f);
	bool show_demo_window = true;
	bool show_another_window = false;
	ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

}
void Window::ImGuiRefresh(glm::vec3 &rotation) {
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	static float f = 0.0f;
	static int counter = 0;

	ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.
	ImGui::SetWindowFontScale(2);

	ImGui::SliderFloat3("translation", &rotation.x, -1.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0fton"))                            // Buttons return true when clicked (most widgets return true when edited/activated)

	ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	ImGui::End();

}
void Window::SetMouseCallback(void(*func)(GLFWwindow*,double,double)) {

	glfwSetCursorPosCallback(window, func);
}

Window::~Window()
{
}
