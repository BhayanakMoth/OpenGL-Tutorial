#include "GLIntferace.h"
#include "ErrorLog.h"
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include<exception>
#include<glm/gtx/transform.hpp>
#include<ctime>
#include<chrono>
#include "VertexBuffer.hpp"
#include "VertexArray.hpp"
#include "Shader.h"
#include "Camera.h"
#include "World.h"
#include "Callbacks.h"
#include "Sphere.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "Texture.h"
void drawBuffers();
GLuint LoadShaders(const char* vertex_file_path, const char* fragment_file_path);
VertexBuffer buffer;
VertexBuffer buffer2;
VertexBuffer buffer3;
Texture texbuff;
Sphere * sphere;
Sphere * sphere2;
Sphere * sphere3;

GLuint programID = 0;
const int width = 800;
const int height = 600;
GLuint ibo = 0;
GLuint vao = 0;
GLuint vbo = 0;
GLuint tex = 0;
void printVector(glm::vec4 vector);


class TimeIt
{
public:
	std::chrono::system_clock::duration m_Duration;
	std::chrono::system_clock::time_point m_TimeBegin;
	std::chrono::system_clock::time_point m_TimeEnd;
	void Start()
	{
		m_TimeBegin = std::chrono::system_clock::now();
	}
	void Stop()
	{
		m_TimeEnd = std::chrono::system_clock::now();
		m_Duration = m_TimeEnd - m_TimeBegin;
	}
	void Duration()
	{
		std::cout << m_Duration.count();
	}
};
TimeIt timeit;



static const GLfloat triangle[] =
{
	-0.5f,-0.5f,0.0f,0.0f, 0.0f, 
	0.0f,0.5f,0.0f,	 0.50f, 1.0f,
	0.5f,-0.5f,0.0f, 1.0f, 0.0f  

};

const GLuint indices[] =
{
	0,1,2
};

void error()
{
	GLenum err = glGetError();
	if (err != GL_NO_ERROR)
	{
		std::string error;
		switch (err) {
		case GL_INVALID_OPERATION:      error = "INVALID_OPERATION";      break;
		case GL_INVALID_ENUM:           error = "INVALID_ENUM";           break;
		case GL_INVALID_VALUE:          error = "INVALID_VALUE";          break;
		case GL_OUT_OF_MEMORY:          error = "OUT_OF_MEMORY";          break;
		case GL_INVALID_FRAMEBUFFER_OPERATION:  error = "INVALID_FRAMEBUFFER_OPERATION";  break;
		}
		std::cout << "GL_" << error.c_str();
		err = glGetError();
	}
}
glm::vec3 ToGLCoordinates(float x, float y, float z)
{
	glm::vec3 vec;
	vec.x = (x / width)*2.0f-1.0f;
	vec.y = (y / height) * 2.0f - 1.0f;
	vec.z = 0.0f;
	return vec;
}


void printVector(glm::vec4 vect4)
{
	std::cout << vect4.x << " " << vect4.y << " " << vect4.z << " " << vect4.w<<"\n";
}
int main()
{
	int err = 0;
	GLI::GLSetup glsetup = GLI::GLSetup(&err);
	auto window = glsetup.CreateWindow(width, height, "Mic Check.");
	glsetup.SetGLFWWindow(4, 3, 3, GLFW_OPENGL_CORE_PROFILE);
	glfwMakeContextCurrent(window);
	glewExperimental = true;
	if (glewInit() != GLEW_OK) {
		std::cout << "Couldn't init glew.";
		return 0;
	}
	glfwSetCursorPosCallback(window, Callbacks::mousePositionCallback);
	glfwSetMouseButtonCallback(window, Callbacks::mouseButtonCallback);
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
	
	Shader shader;
	shader.LoadShader("vertexShader.glsl",ShaderType::VERTEX);
	shader.LoadShader("fragmentShader.glsl",ShaderType::FRAGMENT);
	shader.LoadProgram();
	glUseProgram(shader.GetProgramID());
	glPointSize(10);
	int imgwidth, imgheight, nrChannels;
	unsigned char * imgData = stbi_load("green.png", &imgwidth, &imgheight, &nrChannels, 0);
	
	glm::vec3 pos1 = glm::vec3(0.50f, 0.0f, 0.0f);
	glm::vec3 pos2 = glm::vec3(0.50f, 0.0f, 0.150f);
	glm::vec3 pos3 = glm::vec3(0.50f, 0.0f, 0.250f);
	glm::vec4 color = glm::vec4(0.0, 1.0, 0.0, 0.2);
	glm::vec4 color2 = glm::vec4(1.0, 0.0, 0.0, 0.2);
	sphere = new Sphere(pos1,color);
	sphere->SetShader(shader);
	sphere2 = new Sphere(pos2, color2);
	sphere2->SetShader(shader);
	
	float fps = 0.0f;
	Camera::InitCamera();
	programID = shader.GetProgramID();
	glUniformMatrix4fv(glGetUniformLocation(programID, "model"), 1, GL_FALSE, glm::value_ptr(World::model));
	glUniformMatrix4fv(glGetUniformLocation(programID,"view"), 1, GL_FALSE, glm::value_ptr(Camera::view));
	glUniformMatrix4fv(glGetUniformLocation(programID,"projection"), 1, GL_FALSE, glm::value_ptr(Camera::projection));
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glClearColor(0.05, 0.01, 0.20,0.50);
	

	do{
		timeit.Start();
		glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
		drawBuffers();
		glfwSwapBuffers(window);
		glfwPollEvents();
		timeit.Stop();
		fps = (float)1000000/timeit.m_Duration.count();
	    std::stringstream ss;
		ss << "FPS: " << fps;
		glfwSetWindowTitle(window, ss.str().c_str());
	}while( glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS && glfwWindowShouldClose(window) == 0);

	
	return 0;
}

void Callbacks::update(float theta,float phi)
{
	Camera::updateCamera(phi,theta);
	glUniformMatrix4fv(glGetUniformLocation(programID, "view"), 1, GL_FALSE, glm::value_ptr(Camera::view));

}

void drawBuffers()
{
	sphere->Draw();
	sphere2->Draw();
}

//buffer.Bind();
////GLuint progID = LoadShaders("vertexShader.glsl", "fragmentShader.glsl");
//glVertexAttribPointer(
//	0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
//	3,                  // size
//	GL_FLOAT,           // type
//	GL_FALSE,           // normalized?
//	sizeof(float) * 3,  // stride
//	(void*)0            // array buffer offset
//);
//glDrawArrays(GL_LINES, 0, 24);
//glDrawArrays(GL_POINTS, 0, 24);
//buffer.Unbind();
//
//buffer2.Bind();
//glVertexAttribPointer(
//	0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
//	3,                  // size
//	GL_FLOAT,           // type
//	GL_FALSE,           // normalized?
//	sizeof(float) * 3,  // stride
//	(void*)0            // array buffer offset
//);
//glDrawArrays(GL_LINES, 0, 6);
//buffer2.Unbind();
