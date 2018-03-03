#include "Object.h"

std::vector<Mesh*> loadedMesh;

Object::Object(const char* path) : Active(true) {
	int index = 0; // index use mesh[index] when loading the model

	// TODO save and load from vectors if path == path

	t_mesh = new Mesh;
	t_mesh->m_position = glm::vec3(0.f);
	t_mesh->m_scale = glm::vec3(1.f);
	t_mesh->m_rotation = glm::vec3(.0f);
	std::cout << "Loading mesh path: " << path << std::endl;
	t_mesh->m_path = path;

	loadedMesh.push_back(t_mesh);

	while (1) {
		subMesh* s_mesh = new subMesh;
		if (!loadAssImp(path, s_mesh->Indices, s_mesh->Vertices, s_mesh->UVs, s_mesh->Normals, s_mesh->Tangents, s_mesh->BiTangents, index)) {
			std::cout << index << " meshes loaded." << std::endl;
			return;
		}
		index++;
		CreateBuffers(s_mesh);
		t_mesh->meshes.push_back(s_mesh);
	}
}

void Object::CreateBuffers(subMesh* s_mesh){

	// Gen VAO
	glGenVertexArrays(1, &s_mesh->VAO);
	glBindVertexArray(s_mesh->VAO);

	// Add buffers 
	s_mesh->VBOs.push_back(genVBO(s_mesh, s_mesh->Vertices.size() * sizeof(glm::vec3), 3, &s_mesh->Vertices[0], GL_STATIC_DRAW));

	if (!s_mesh->UVs.empty())
		s_mesh->VBOs.push_back(genVBO(s_mesh, s_mesh->UVs.size() * sizeof(glm::vec2), 2, &s_mesh->UVs[0], GL_STATIC_DRAW));

	if (!s_mesh->Normals.empty())
		s_mesh->VBOs.push_back(genVBO(s_mesh, s_mesh->Normals.size() * sizeof(glm::vec3), 3, &s_mesh->Normals[0], GL_STATIC_DRAW));

	if (!s_mesh->Tangents.empty())
		s_mesh->VBOs.push_back(genVBO(s_mesh, s_mesh->Tangents.size() * sizeof(glm::vec3), 3, &s_mesh->Tangents[0], GL_STATIC_DRAW));

	if (!s_mesh->BiTangents.empty())
		s_mesh->VBOs.push_back(genVBO(s_mesh, s_mesh->BiTangents.size() * sizeof(glm::vec3), 3, &s_mesh->BiTangents[0], GL_STATIC_DRAW));

	// Add element buffer
	if (!s_mesh->Indices.empty()) {
		glGenBuffers(1, &s_mesh->EBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, s_mesh->EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, s_mesh->Indices.size() * sizeof(unsigned short), &s_mesh->Indices[0], GL_STATIC_DRAW);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, s_mesh->EBO);
		s_mesh->drawSize = s_mesh->Indices.size();
	}
}

// Return a new VBO for each call
GLuint Object::genVBO(subMesh* m_mesh, GLsizeiptr size, GLint dataSize, const void* data, GLenum usage) {

	glBindVertexArray(m_mesh->VAO);

	GLuint buffer;
	glGenBuffers(1, &buffer);
	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	glBufferData(GL_ARRAY_BUFFER, size, data, usage);

	glEnableVertexAttribArray(m_mesh->VBOs.size());
	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	glVertexAttribPointer(m_mesh->VBOs.size(), dataSize, GL_FLOAT, GL_FALSE, 0, 0);

	return buffer;
}

void Object::Draw(Shader* s_Shader, glm::mat4 ViewMatrix_) {
	for (unsigned int i = 0; i < this->t_mesh->meshes.size(); i++) {
		t_mesh->meshes[i]->S_Draw(ViewMatrix_, s_Shader, this->t_mesh->m_position, this->t_mesh->m_rotation, this->t_mesh->m_scale);
	}
}

void Object::addTexture(Texture* texture) {
	for(unsigned int i = 0; i < t_mesh->meshes.size(); i++)
		t_mesh->meshes[i]->m_texture.push_back(texture);
}

void Object::Destroy() { // TODO
	for (unsigned int i = 0; i < t_mesh->meshes.size(); i++) {
		t_mesh->meshes[i]->Destroy();
		delete t_mesh;
	}
}