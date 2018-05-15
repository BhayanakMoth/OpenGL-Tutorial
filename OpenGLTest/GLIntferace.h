#pragma once
#include<GL\glew.h>
#include<stdio.h>
#include<stdlib.h>
#include<GLFW/glfw3.h>
#include<glm/glm.hpp>
#include<iostream>
#include "ErrorLog.h"

class GLSetup
{
public:
	 GLSetup(int * err);
	void  SetGLFWWindow(short samples, short major, short minor,  int profile,bool forwardCompatibility = true) const;
	GLFWwindow * CreateWindow(int width, int height, std::string name) const;

	
};