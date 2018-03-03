#pragma once

#include <iostream>
#include <functional>

#include <GL\glew.h>
#include <GLFW\glfw3.h>
#include <glm\glm.hpp>

#include "CubeMap.h"
#include "Camera.h"
#include "Object.h"
#include "Light.h"

#include "../Shader/Shader.h"

class Scene {
public:
	Scene();
	inline void setResolution(glm::vec2 res) { this->resolution = res; }
	inline void setResolution(float x, float y) { this->resolution.x = x; this->resolution.y = y; }
	inline void UpdateCallback(std::function<void()> update) { this->update_cb = update; }
	inline void addCubeMap(CubeMap* cube) { c_map.push_back(cube); }
	inline void bindShader(Shader* Shader) { this->scene_shader = Shader; }
	inline void addObject(Object* obj) { this->s_obj.push_back(obj); }
	inline void addLight(Light* light) { this->s_light.push_back(light); }
	inline GLFWwindow* getWindow() { return this->t_window; }
	inline Camera* getCamera(); // TODO
	inline glm::vec2 getResolution() { return this->resolution; }

	void Start();
	int Init();
	void Delete();
private:
	glm::vec2 resolution;
	std::function<void()> update_cb;
	Shader* scene_shader;
	std::vector<Object*> s_obj; // All scene objects
	std::vector<CubeMap*> c_map;
	std::vector<Light*> s_light; // All scene light objects
	GLFWwindow* t_window;
	void Draw();
};

