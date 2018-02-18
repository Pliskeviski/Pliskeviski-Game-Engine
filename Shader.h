#pragma once
#include <iostream>
#include <vector>

#include <GL\glew.h>

#include "stringLoader.hpp"


class Shader {
public:
	Shader() {};
	static GLuint CreateShader(std::string vertexPath, std::string fragmentPath) {
		std::string vertexShader = load(vertexPath);
		std::string fragmentShader = load(fragmentPath);

		if (fragmentPath.empty() || vertexPath.empty())
			return NULL;

		GLuint vShader = glCreateShader(GL_VERTEX_SHADER), fShader = glCreateShader(GL_FRAGMENT_SHADER); // Vertex and fragment shader

																										 /* compile stuff */
		GLint Result = GL_FALSE;
		int InfoLogLength;

		// Compile vertex shaders
		const char* vertexShaderSource = vertexShader.c_str();
		glShaderSource(vShader, 1, &vertexShaderSource, NULL);
		glCompileShader(vShader);
		// Check Vertex Shader
		glGetShaderiv(vShader, GL_COMPILE_STATUS, &Result);
		glGetShaderiv(vShader, GL_INFO_LOG_LENGTH, &InfoLogLength);
		if (InfoLogLength > 0) {
			std::vector<char> VertexShaderErrorMessage(InfoLogLength + 1);
			glGetShaderInfoLog(vShader, InfoLogLength, NULL, &VertexShaderErrorMessage[0]);
			printf("%s\n", &VertexShaderErrorMessage[0]);
		}

		// fragment shader
		const char* fragmentShaderSource = fragmentShader.c_str();
		glShaderSource(fShader, 1, &fragmentShaderSource, NULL);
		glCompileShader(fShader);
		// Check Vertex Shader
		glGetShaderiv(fShader, GL_COMPILE_STATUS, &Result);
		glGetShaderiv(fShader, GL_INFO_LOG_LENGTH, &InfoLogLength);
		if (InfoLogLength > 0) {
			std::vector<char> VertexShaderErrorMessage(InfoLogLength + 1);
			glGetShaderInfoLog(fShader, InfoLogLength, NULL, &VertexShaderErrorMessage[0]);
			printf("%s\n", &VertexShaderErrorMessage[0]);
		}

		GLuint shaderID = glCreateProgram();
		glAttachShader(shaderID, vShader);
		glAttachShader(shaderID, fShader);
		glLinkProgram(shaderID);

		// Check the program
		glGetProgramiv(shaderID, GL_LINK_STATUS, &Result);
		glGetProgramiv(shaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
		if (InfoLogLength > 0) {
			std::vector<char> ProgramErrorMessage(InfoLogLength + 1);
			glGetProgramInfoLog(shaderID, InfoLogLength, NULL, &ProgramErrorMessage[0]);
			printf("%s\n", &ProgramErrorMessage[0]);
		}

		glDetachShader(shaderID, vShader);
		glDetachShader(shaderID, fShader);

		glDeleteShader(vShader);
		glDeleteShader(fShader);

		return shaderID;

	}
};

