#pragma once
#include<GL/glew.h>
#include<GLFW/glfw3.h>
#include<fstream>
#include<sstream>
#include<iostream>
#include<vector>
enum ShaderType
{
	VERTEX,
	FRAGMENT,
	GEOMETRY
};
class Shader
{
private:
	GLuint m_ProgID;
	GLuint m_VertID;
	GLuint m_FragID;
	GLuint m_GeomID;
public:
	Shader()
		:
		m_ProgID(0),
		m_VertID(0),
		m_FragID(0),
		m_GeomID(0)
	{

	}
	void LoadProgram()
	{
		std::cout << "Linking the program.\n";
		m_ProgID = glCreateProgram();
		if (m_VertID != 0)
			glAttachShader(m_ProgID, m_VertID);
		if (m_FragID != 0)
			glAttachShader(m_ProgID, m_FragID);
		if (m_GeomID != 0)
			glAttachShader(m_ProgID, m_GeomID);

		glLinkProgram(m_ProgID);

		GLint l_Result = 0;
		GLint l_LogLength = 0;
		glGetProgramiv(m_ProgID, GL_LINK_STATUS, &l_Result);
		glGetProgramiv(m_ProgID, GL_INFO_LOG_LENGTH, &l_LogLength);

		if (l_LogLength > 0)
		{
			std::vector<char> error(l_LogLength + 1);
			glGetProgramInfoLog(m_ProgID, l_LogLength, NULL, &error[0]);
			std::cout << (std::string(&error[0]));
		}

		if (m_VertID != 0)
			glDetachShader(m_ProgID, m_VertID);
		if (m_FragID != 0)
			glDetachShader(m_ProgID, m_FragID);
		if (m_GeomID != 0)
			glDetachShader(m_ProgID, m_GeomID);
	}
	void LoadShader(const char * p_Path, ShaderType p_Type)
	{
		std::ifstream l_InFileStream(p_Path, std::ios::in);
		std::stringstream l_SS;
		if (l_InFileStream.is_open())
		{
			l_SS << l_InFileStream.rdbuf();
			l_InFileStream.close();
		}
		else
		{
			std::cout << "Error Loading Shaders: Cannot open file stream.";
			return;
		}
		std::string l_Src = l_SS.str();
		const char * l_ShaderSrc = l_Src.c_str();
		GLuint * l_ID = nullptr;
		switch (p_Type)
		{
		case ShaderType::FRAGMENT:
			m_FragID = glCreateShader(GL_FRAGMENT_SHADER);
			l_ID = &m_FragID;
			break;
		case ShaderType::GEOMETRY:
			m_GeomID = glCreateShader(GL_GEOMETRY_SHADER);
			l_ID = &m_GeomID;
			break;
		case ShaderType::VERTEX:
			m_VertID = glCreateShader(GL_VERTEX_SHADER);
			l_ID = &m_VertID;
			break;
		}
		GLint l_Result = GL_FALSE;
		GLint l_LogLength = 0;
		glShaderSource(*l_ID, 1, &l_ShaderSrc, NULL);
		glCompileShader(*l_ID);
		glGetShaderiv(*l_ID, GL_COMPILE_STATUS, &l_Result);
		glGetShaderiv(*l_ID, GL_INFO_LOG_LENGTH, &l_LogLength);

		if (l_LogLength > 0)
		{
			std::vector<char> error(l_LogLength + 1);
			glGetShaderInfoLog(*l_ID, l_LogLength, NULL, &error[0]);
			std::cout << (std::string(&error[0]));
		}

	}
	GLuint GetProgramID() const
	{
		return m_ProgID;
	}
	GLuint GetShader(ShaderType p_Type) const
	{
		GLuint l_RetnVal = -1;
		switch (p_Type)
		{
		case ShaderType::FRAGMENT:
			l_RetnVal = m_FragID;
			break;
		case ShaderType::GEOMETRY:
			l_RetnVal = m_GeomID;
			break;
		case ShaderType::VERTEX:
			l_RetnVal = m_VertID;
			break;
		}
		return l_RetnVal;
	}
};