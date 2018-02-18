#include "Object.h"

std::vector<Vertex> loadedVertex;

Object::Object(std::string path, int fixRotation) {
	this->position = glm::vec3(.0f, .0f, .0f);
	this->scale = glm::vec3(1.f);

	for (int i = 0; i < loadedVertex.size(); i++) {
		if (loadedVertex[i].path == path)
			this->obj = loadedVertex[i];
	}

	// Get the information from the file
	if (this->obj.Vertices.empty()) {
		//loadAssImp(path.c_str(), this->obj.Indices, this->obj.Vertices, this->obj.UVs, this->obj.Normals);
		readPlis(path, this->obj.Indices, this->obj.Vertices, this->obj.UVs, this->obj.Normals);
		this->obj.path = path;
		loadedVertex.push_back(this->obj);
	}

	CreateBuffers(fixRotation);
}

Object::Object(std::string path, glm::vec3 pos, int fixRotation) {

	this->position = pos;
	this->scale = glm::vec3(1.f);

	for (int i = 0; i < loadedVertex.size(); i++) {
		if (loadedVertex[i].path == path) 
			this->obj = loadedVertex[i];
	}

	// Get the information from the file
	if (this->obj.Vertices.empty()) {
		//loadAssImp(path.c_str(), this->obj.Indices, this->obj.Vertices, this->obj.UVs, this->obj.Normals);
		readPlis(path, this->obj.Indices, this->obj.Vertices, this->obj.UVs, this->obj.Normals);
		this->obj.path = path;
		loadedVertex.push_back(this->obj);
	}

	CreateBuffers(fixRotation);
}

void Object::CreateBuffers(int fixRotation) {

	if(fixRotation != 0)
		setRotationX(1.56f * fixRotation);

	// Gen VAO
	glGenVertexArrays(1, &this->VAO);
	glBindVertexArray(this->VAO);

	// Add buffers 
	this->VBOs.push_back(genVBO(this->obj.Vertices.size() * sizeof(glm::vec3), 3, &this->obj.Vertices[0], GL_STATIC_DRAW));

	if (!this->obj.UVs.empty())
		this->VBOs.push_back(genVBO(this->obj.UVs.size() * sizeof(glm::vec2), 2, &this->obj.UVs[0], GL_STATIC_DRAW));

	if (!this->obj.Normals.empty())
		this->VBOs.push_back(genVBO(this->obj.Normals.size() * sizeof(glm::vec3), 3, &this->obj.Normals[0], GL_STATIC_DRAW));

	// Add element buffer
	if (!this->obj.Indices.empty()) {
		glGenBuffers(1, &this->EBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->obj.Indices.size() * sizeof(unsigned short), &this->obj.Indices[0], GL_STATIC_DRAW);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->EBO);
		this->drawSize = this->obj.Indices.size();
	}
}

GLuint Object::genVBO(GLsizeiptr size, GLint dataSize, const void* data, GLenum usage) {

	glBindVertexArray(this->VAO);

	GLuint buffer;
	glGenBuffers(1, &buffer);
	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	glBufferData(GL_ARRAY_BUFFER, size, data, usage);

	glEnableVertexAttribArray(this->VBOs.size());
	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	glVertexAttribPointer(this->VBOs.size(), dataSize, GL_FLOAT, GL_FALSE, 0, 0);

	return buffer;
}

void Object::Draw() {
	glUseProgram(this->Shader);
	glBindVertexArray(this->VAO);

	// Enable VBOs
	for (int i = 0; i < this->VBOs.size(); i++) {
		glEnableVertexAttribArray(i);
	}

	for (unsigned int i = 0; i < this->textures.size(); i++) {
		glActiveTexture(GL_TEXTURE0+i);

		std::string name = "Texture";
		std::string number;

		number = std::to_string(i);

		name.append(number);

		glUniform1i(glGetUniformLocation(Shader, name.c_str()), i);
		glBindTexture(GL_TEXTURE_2D, this->textures[i]);
	}

	GLuint ViewID = glGetUniformLocation(this->Shader, "View");
	GLuint ProjectionID = glGetUniformLocation(this->Shader, "Projection");
	GLuint ModelID = glGetUniformLocation(this->Shader, "Model");

	glm::mat4 ProjectionMatrix = glm::perspective(45.f, (GLfloat)800 / (GLfloat)600, 0.1f, 1000.0f);
	glm::mat4 ViewMatrix = getViewMatrix();
	glm::mat4 ModelMatrix = glm::translate(glm::mat4(1.f), this->position);
	glm::mat4 ModelRX = glm::rotate(glm::mat4(1.f), rotation.x, glm::vec3(1.0f, .0f, .0f));
	glm::mat4 ModelRY = glm::rotate(glm::mat4(1.f), rotation.y, glm::vec3( .0f,1.0f, .0f));
	glm::mat4 ModelRZ = glm::rotate(glm::mat4(1.f), rotation.z, glm::vec3( .0f, .0f,1.0f));
	glm::mat4 Model3 = glm::scale(glm::mat4(1.f), scale);
	glm::mat4 MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;

	glm::mat4 ModelRotation = ModelRX * ModelRY * ModelRZ;

	ModelMatrix *= ModelRotation * Model3;

	glUniformMatrix4fv(ViewID, 1, GL_FALSE, glm::value_ptr(ViewMatrix));
	glUniformMatrix4fv(ModelID, 1, GL_FALSE, glm::value_ptr(ModelMatrix));
	glUniformMatrix4fv(ProjectionID, 1, GL_FALSE, glm::value_ptr(ProjectionMatrix));



	glDrawElements(GL_TRIANGLES, drawSize, GL_UNSIGNED_SHORT, (void*)0);

	// Disable VBOs
	for (int i = 0; i < this->VBOs.size(); i++) {
		glDisableVertexAttribArray(i);
	}
	glUseProgram(0);
}

void Object::Destroy() {
	for (unsigned int i = 0; i < this->VBOs.size(); i++)
		glDeleteBuffers(1, &this->VBOs[i]);

	glDeleteVertexArrays(1, &VAO);
	for (unsigned int i = 0; i < this->textures.size(); i++)
		glDeleteTextures(1, &textures[i]);

}
