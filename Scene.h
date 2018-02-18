#pragma once
#include <iostream>

#include <vector>
#include <GL\glew.h>
#include <GLFW\glfw3.h>

#include "Object.h"
#include "controls.hpp"

class Scene {
public:
	Scene();
	int Init();
	void draw(GLuint shader);
	void Destroy();
	inline void setResolution(glm::vec2 res) { resolution = res; };
	inline void addObject(Object* obj) { objects.push_back(obj); };
	GLFWwindow* getWindow();
	
private:
	std::vector<Object*> objects;
	glm::vec2 resolution;
	float distance(float camera, float cube);
};


