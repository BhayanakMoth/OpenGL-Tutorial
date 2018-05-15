#pragma once

#include "GLIntferace.h"
GLSetup::GLSetup(int * err)
{
	if (!glfwInit())
	{
		LogError("Fatal Error: glfw failed to initialize. ");
		*err = -1;
	}
	*err = 0;
}
void GLSetup::SetGLFWWindow(short samples, short major, short minor, int profile, bool forwardCompatibility) const
{
	glfwWindowHint(GLFW_SAMPLES, samples);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, major);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, minor);
	if (forwardCompatibility == true)
		glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	else
		glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_FALSE);

	glfwWindowHint(GLFW_OPENGL_PROFILE, profile);

}
GLFWwindow * GLSetup::CreateWindow(int width, int height, std::string title) const
{
	GLFWwindow * window;
	window = glfwCreateWindow(width, height, title.c_str(), NULL, NULL);
	return window;
}
