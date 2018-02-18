#pragma once
#include <vector>
#include <sstream>

#include <GL\glew.h>

#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>

#include "PlisReader.h"
#include "Texture.h"
#include "controls.hpp"

struct Vertex {
	std::string path;

	std::vector<glm::vec3> Vertices;
	std::vector<glm::vec3> Normals;
	std::vector<glm::vec2> UVs;
	std::vector<unsigned short> Indices;
	// TODO: Tangents, binormals etc
};

class Object {
public:
	Object() {};
	Object(std::string path, int fixRotation);
	Object(std::string path, glm::vec3 pos, int fixRotation);
	void Draw();
	void CreateBuffers(int fixRotation);

	void setRotationX(float angle) { rotation.x = angle; };
	void setRotationY(float angle) { rotation.y = (angle * 1.56f); };
	void setRotationZ(float angle);
	
	inline void setScale(float scale) { this->scale = glm::vec3(scale); };
	inline void bindShader(GLuint shader) { this->Shader = shader; };
	inline void addTexture(Texture textureID) { textures.push_back(textureID.getID()); };
	inline void setPosition(glm::vec3 pos) { this->position = pos; };
	inline Vertex getVertex() { return obj; }
	inline void setVertex(Vertex vert) { obj = vert; }
	inline glm::vec3 getPosition() { return position; }

	void Destroy();
private:
	std::vector<GLuint> textures;
	glm::vec3 position;
	glm::vec3 rotation;
	glm::vec3 scale;

	GLuint VAO; // Vertex array
	std::vector<GLuint> VBOs; // Vertex Buffers
	GLuint EBO; // Index
	Vertex obj;
	GLuint Shader;

	unsigned int drawSize;

	GLuint genVBO(GLsizeiptr size, GLint dataSize, const void* data, GLenum usage);
};

