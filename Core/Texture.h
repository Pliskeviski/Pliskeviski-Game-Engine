#pragma once

#include <iostream>
#include <vector>
#include <string>
#include <assert.h>

#include <GL\glew.h>

#include <SOIL2\SOIL2.h>

enum TextureType {
	TYPE_CUBEMAP,
	TYPE_DIFFUSE,
	TYPE_METALLIC,
	TYPE_ROUGHNESS,
	TYPE_NORMAL,
	TYPE_AO 
};

struct m_Texture {
	GLuint id;
	TextureType type;
};

class Texture {
public:
	Texture(std::string path, TextureType type);
	Texture(std::vector<std::string> faces, TextureType type);
	inline GLuint getID() { return texture.id; }
	inline TextureType getType() { return texture.type; }
	inline void Delete() { glDeleteTextures(1, &texture.id); }
private:
	GLuint loadTexture(std::string path);
	GLuint loadCubeMap(std::vector<std::string> faces);
	m_Texture texture;
};

