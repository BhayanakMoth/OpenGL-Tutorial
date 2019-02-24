#pragma once
#include<iostream>
#include<cstdlib>
#include<string>
#include<fstream>
#include<sstream>
#include<GL/glew.h>
#include<GLFW/glfw3.h>
#include<GL/GL.h>
#include<GL/GLU.h>
#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>

#define NUM_BODIES 400
GLuint _LoadShaders(const char * vertex_file_path, const char * fragment_file_path)
{
	//Creating Shaders
	GLuint vertexShadID = glCreateShader(GL_VERTEX_SHADER);
	GLuint fragmentShadID = glCreateShader(GL_FRAGMENT_SHADER);

	//Read the Vertex Shader code from the file
	std::string VertexShaderCode;
	std::ifstream VertexShaderStream(vertex_file_path, std::ios::in);

	if (VertexShaderStream.is_open())
	{
		std::stringstream sstr;
		sstr << VertexShaderStream.rdbuf();
		VertexShaderCode = sstr.str();
		VertexShaderStream.close();

	}
	else
	{
		LogError("Impossible to open vertexshader.");
		return 0;
	}
	//Reading the fragment shader code from the file
	std::string FragmentShaderCode;
	std::ifstream FragmentShaderStream(fragment_file_path, std::ios::in);
	if (FragmentShaderStream.is_open())
	{
		std::stringstream sstr;
		sstr << FragmentShaderStream.rdbuf();
		FragmentShaderCode = sstr.str();
		FragmentShaderStream.close();
	}

	GLint Result = GL_FALSE;
	int infoLogLength;

	//Compiling Vertex Shader
	std::cout << "Compiling shader: " + std::string(vertex_file_path) + " \n";
	const char * VertexSourcePointer = VertexShaderCode.c_str();
	glShaderSource(vertexShadID, 1, &VertexSourcePointer, NULL);
	glCompileShader(vertexShadID);


	glGetShaderiv(vertexShadID, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(vertexShadID, GL_INFO_LOG_LENGTH, &infoLogLength);

	if (infoLogLength > 0)
	{
		std::vector<char> error(infoLogLength + 1);
		glGetShaderInfoLog(vertexShadID, infoLogLength, NULL, &error[0]);
		LogError(std::string(&error[0]));
	}

	std::cout << "Compiling FragShaders: " + std::string(fragment_file_path) + "\n";
	char const * fragSourcePointer = FragmentShaderCode.c_str();
	glShaderSource(fragmentShadID, 1, &fragSourcePointer, NULL);
	glCompileShader(fragmentShadID);

	glGetShaderiv(fragmentShadID, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(fragmentShadID, GL_INFO_LOG_LENGTH, &infoLogLength);

	if (infoLogLength > 0)
	{
		std::vector<char> error(infoLogLength + 1);
		glGetShaderInfoLog(fragmentShadID, infoLogLength, NULL, &error[0]);
		LogError(std::string(&error[0]));
	}


	//Link the program
	std::cout << "Linking the program.\n";
	GLuint ProgramID = glCreateProgram();
	glAttachShader(ProgramID, vertexShadID);
	glAttachShader(ProgramID, fragmentShadID);
	glLinkProgram(ProgramID);

	glGetProgramiv(ProgramID, GL_LINK_STATUS, &Result);
	glGetProgramiv(ProgramID, GL_INFO_LOG_LENGTH, &infoLogLength);
	if (infoLogLength > 0)
	{
		std::vector<char> error(infoLogLength + 1);
		glGetProgramInfoLog(ProgramID, infoLogLength, NULL, &error[0]);
		LogError(std::string(&error[0]));
	}

	glDetachShader(ProgramID, vertexShadID);
	glDetachShader(ProgramID, fragmentShadID);

	glDeleteShader(vertexShadID);
	glDeleteShader(fragmentShadID);
	return ProgramID;
}

static void KeyCallback(GLFWwindow * p_Window, int p_Key, int p_Scancode, int p_Action, int p_Mods)
{
	if (p_Key == GLFW_KEY_ESCAPE && p_Action == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(p_Window, GL_TRUE);
	}
}
class ScreenProps
{
public:
	int width = 0;
	int height = 0;
	std::string dev_name = " ";
};
class VAO
{


private: friend class Triangle;
		 GLfloat *m_bodies;
		 GLfloat *m_bindices;
		 GLuint m_VAO;
		 GLuint m_VBO;
public:
	void LoadVertexBuffer(GLfloat * p_Data, GLsizei p_Size)
	{

		assert(p_Data != NULL);
		glGenVertexArrays(1, &m_VAO);
		/*Param: Handle to VAO*/
		glBindVertexArray(m_VAO);
		glGenBuffers(1, &m_VBO);
		glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
		glBufferData(GL_ARRAY_BUFFER, p_Size, p_Data, GL_STATIC_DRAW);

		//Loads the color data into the GPU
	}
	void Draw()
	{

	}
};
class Triangle
{
private:
	VAO m_VAO;

public:
	Triangle() {
		GLfloat l_Vertices[9] = {
		-1.0f,-1.0f,0.0f,
		-0.0f,1.0f,0.0f,
		1.0f,-1.0f,0.0f
		};
		m_VAO = VAO();
		m_VAO.LoadVertexBuffer(l_Vertices, sizeof(GLfloat) * 9);

	}
	void Draw()
	{
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, m_VAO.m_VBO);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, (const void*)0);
		glDrawArrays(GL_TRIANGLES, 0, 3);
	}
};
class Shader
{
public:
	
	GLuint m_Program;
	Shader()
	{
		m_Program = _LoadShaders("vertexShader.glsl","fragmentShader.glsl");
		
	}

};

void initVAO(VAO & vao)
{
	//GLfloat *bodies = new GLfloat[4 * (NUM_BODIES)];
	//GLuint *bindices = new GLuint[NUM_BODIES];
	//glm::vec4 ul(-1.0f, -1.0f, 1.0, 1.0);
	//glm::vec4 lr(1.0, 1.0, 0.0, 0.0);
	/*for (int i = 0; i < NUM_BODIES; i++)
	{
	}*/
	GLfloat l_Vertices[9] = {
		-1.0f,-1.0f,0.0f,
		0.0f,1.0f,0.0f,
		1.0f,-1.0f,0.0f
	};
	vao.LoadVertexBuffer(l_Vertices, sizeof(float) * 9);
}

int InitOpenGL()
{
	//cudaDeviceProp deviceprop;
	int gpuDrive = 0;
	VAO vao;
	if (!glfwInit())
	{
		std::cout << "OpenGL nahi chal raha...";
		return 0;
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	ScreenProps scrprops;
	scrprops.width = 800;
	scrprops.height = 600;
	GLFWwindow * window = glfwCreateWindow(scrprops.width, scrprops.height, scrprops.dev_name.c_str(), NULL, NULL);
	if (!window)
	{
		std::cout << "Window nahi ban raha..";
		glfwTerminate();
		return 0;
	}
	glfwMakeContextCurrent(window);
	glfwSetKeyCallback(window, KeyCallback);
	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK)
	{
		return false;
	}
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
	//cudaGLSetGLDevice(0);

	Triangle triangle = Triangle();

	Shader shader = Shader();
	glUseProgram(shader.m_Program);

	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents();
		triangle.Draw();
		glfwSwapBuffers(window);

	}
	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}