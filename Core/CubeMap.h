#pragma once
#include <vector>

#include <GL\glew.h>

#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>

#include <SOIL2\SOIL2.h>

#include "../Shader/Shader.h"
#include "Camera.h"
#include "Texture.h"

class CubeMap {
public:
	CubeMap();
	inline void bindTexture(Texture* texture) { this->texture = texture; }
	inline void setActive(bool active) { this->Active = active; }
	inline bool isActive() { return this->Active; }
	void Draw(Camera* camera);
	void Destroy();
private:
	GLuint Shader_ID;
	GLuint VAO;
	GLuint VBO;
	Texture* texture;
	bool Active;
};

