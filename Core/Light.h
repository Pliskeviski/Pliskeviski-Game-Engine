#pragma once

#include <string>

#include <GL\glew.h>
#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>

#include "Object.h"

#include "../Shader/Shader.h"

class Light {
public:
	Light(Shader* Scene_Shader);
	Light(Shader* Scene_Shader, glm::vec3 pos);
	Light(Shader* Scene_Shader, glm::vec3 pos, glm::vec3 color);
	
	int CompileShader(int v);
	
	void Draw(glm::mat4 viewMatrix);
	void setColor(glm::vec3 color);
	void setIntensity(float i) { this->intensity = i; this->color * glm::vec3(this->intensity); }
	
	inline void setPosition(glm::vec3 pos) { this->pos = pos; }
	inline void setActive(bool active) { this->Active = active; }
	inline void setDebug(bool debug) { this->Debug = debug; }
	
	inline float getIntensity() { return this->intensity; }
	inline bool isActive() { return this->Active; }
	
	static void Delete();

private:
	void Create();
	glm::vec3 pos;
	glm::vec3 color;
	Shader* s_shader;
	GLuint VAO;
	GLuint VBO;
	GLuint EBO;
	GLuint newShader;
	Shader* this_Shader;
	float Scale;
	float intensity;
	unsigned int index;
	unsigned int drawSize;
	bool Debug;
	bool Active;
};

