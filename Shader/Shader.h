#pragma once
#include <iostream>
#include <vector>

#include <GL\glew.h>

#include "stringLoader.hpp"


class Shader {
public:
	GLuint shader;
	Shader(std::string vertexPath, std::string fragmentPath) {
		std::string vertexShader = load(vertexPath);
		std::string fragmentShader = load(fragmentPath);
		
		if (fragmentPath.empty() || vertexPath.empty())
			return;

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

		this->shader = shaderID;
		this->path_fragment = fragmentPath;
		this->path_fragment = vertexPath;
	}
	inline void Use() { glUseProgram(this->shader); }
	static void setVec3(Shader* shader, const std::string &name, float v1, float v2, float v3) {
		glUseProgram(shader->shader);
		glUniform3f(glGetUniformLocation(shader->shader, name.c_str()), v1, v2 ,v3);
		glUseProgram(0);
	}
	static void setFloat(Shader* shader, const std::string &name, float v) {
		glUseProgram(shader->shader);
		glUniform1f(glGetUniformLocation(shader->shader, name.c_str()), v);
		glUseProgram(0);
	}
	static void setMat4(Shader* shader, const std::string &name, float v1, float v2, float v3, float v4) {
		glUseProgram(shader->shader);
		glUniform4f(glGetUniformLocation(shader->shader, name.c_str()), v1, v2, v3, v4);
		glUseProgram(0);
	}
private:
	std::string path_fragment;
	std::string path_vertex;
};

