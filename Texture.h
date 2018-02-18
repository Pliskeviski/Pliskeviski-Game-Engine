#pragma once

#include <iostream>
#include <vector>
#include <string>

#include <GL\glew.h>

#include <SOIL2\SOIL2.h>

class Texture {
public:
	Texture(std::string path);
	inline GLuint getID() { return this->id; };
private:
	GLuint loadTexture(std::string path);
	GLuint id;
};

