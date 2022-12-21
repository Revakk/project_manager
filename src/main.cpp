#include <iostream>
#include <vector>
#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "project_manager_gui.hpp"

using namespace pm;
using namespace gui;

int main(int, char**) 
{

	constexpr int width = 500;
	constexpr int height = 700;


   	if (!glfwInit())
		return 1;

	// GL 3.0 + GLSL 130
	const char* glsl_version = "#version 130";
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
	//glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+ only
	//glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            // 3.0+ only

	// Create window with graphics context
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
	GLFWwindow* window = glfwCreateWindow(width, height, "Project manager", NULL, NULL);
	if (window == NULL)
		return 1;
	glfwMakeContextCurrent(window);
	//glfwSwapInterval(3); // Enable vsync

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))  // tie window context to glad's opengl funcs
		throw("Unable to context to OpenGL");

	int screen_width, screen_height;
	glfwGetFramebufferSize(window, &screen_width, &screen_height);
	glViewport(0, 0, screen_width, screen_height);

	project_manager_gui myimgui;
	myimgui.init(window, glsl_version,width,height);
	while (!glfwWindowShouldClose(window)) {
		glfwPollEvents();



		glClear(GL_COLOR_BUFFER_BIT);
		myimgui.new_frame();
		myimgui.update();
		myimgui.render_ui();
		glfwSwapBuffers(window);

	}
	myimgui.shutdown();

	return 0;
}
