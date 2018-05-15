#include "GLIntferace.h"
#include "ErrorLog.h"
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
using namespace glm;
void makeTriangle();
GLuint LoadShaders(const char* vertex_file_path, const char* fragment_file_path);

int main()
{
	int err = 0;
	GLSetup glsetup =  GLSetup(&err);
	if (err != 0)
	{
		return err;
	}
	glsetup.SetGLFWWindow(4, 3, 3, GLFW_OPENGL_CORE_PROFILE, true);
	auto window = glsetup.CreateWindow(800, 600, "Test");
	glfwMakeContextCurrent(window);
	glewExperimental = true;
	if (glewInit() != GLEW_OK)
	{
		fprintf(stderr, "Failed to initialize GLEW. ");
		system("pause");
		return -1;

	}
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
	GLuint programID = LoadShaders("vertexShader.txt", "fragmentShader.txt");
	
	
	do{
		//Clear the screen. Can cause flickering if not done
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		//Draw Nothing, not yet.
		glUseProgram(programID);
		//Swap Buffers
		makeTriangle();
		glfwSwapBuffers(window);
		glfwPollEvents();

	}while( glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS && glfwWindowShouldClose(window) == 0);

	
	return 0;
}
void makeTriangle()
{
	GLuint VertexArrayID;
	glGenVertexArrays(1, &VertexArrayID);
	glBindVertexArray(VertexArrayID);

	static const GLfloat g_vertex_buffer_data[] = {
													  -1.0f,-1.0f,0.0f,
													  1.0f,-1.0f,0.0f,
													  0.0f,1.0f,0.0f };
	GLuint vertexbuff;
	glGenBuffers(1, &vertexbuff);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuff);
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuff);
	glVertexAttribPointer(
		0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
		3,                  // size
		GL_FLOAT,           // type
		GL_FALSE,           // normalized?
		0,                  // stride
		(void*)0            // array buffer offset
	);
	// Draw the triangle !
	glDrawArrays(GL_TRIANGLES, 0, 3); // Starting from vertex 0; 3 vertices total -> 1 triangle
	glDisableVertexAttribArray(0);

}

GLuint LoadShaders(const char * vertex_file_path, const char * fragment_file_path)
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
	//Mic Check
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

	std::cout << "Compiling FragShaders: " + std::string(fragment_file_path)+"\n";
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



