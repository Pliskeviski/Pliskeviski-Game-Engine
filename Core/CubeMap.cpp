#include "CubeMap.h"

CubeMap::CubeMap() : Active(true) {

	std::string vertex_path = "P_Shaders/cubemap.vertexshader";;
	std::string fragment_path = "P_Shaders/cubemap.fragmentshader";

	std::vector<glm::vec3> vertices = {
		glm::vec3(-1.0f,  1.0f, -1.0f),
		glm::vec3(-1.0f, -1.0f, -1.0f),
		glm::vec3(1.0f, -1.0f, -1.0f),
		glm::vec3(1.0f, -1.0f, -1.0f),
		glm::vec3(1.0f,  1.0f, -1.0f),
		glm::vec3(-1.0f,  1.0f, -1.0f),

		glm::vec3(-1.0f, -1.0f,  1.0f),
		glm::vec3(-1.0f, -1.0f, -1.0f),
		glm::vec3(-1.0f,  1.0f, -1.0f),
		glm::vec3(-1.0f,  1.0f, -1.0f),
		glm::vec3(-1.0f,  1.0f,  1.0f),
		glm::vec3(-1.0f, -1.0f,  1.0f),

		glm::vec3(1.0f, -1.0f, -1.0f),
		glm::vec3(1.0f, -1.0f,  1.0f),
		glm::vec3(1.0f,  1.0f,  1.0f),
		glm::vec3(1.0f,  1.0f,  1.0f),
		glm::vec3(1.0f,  1.0f, -1.0f),
		glm::vec3(1.0f, -1.0f, -1.0f),

		glm::vec3(-1.0f, -1.0f,  1.0f),
		glm::vec3(-1.0f,  1.0f,  1.0f),
		glm::vec3(1.0f,  1.0f,  1.0f),
		glm::vec3(1.0f,  1.0f,  1.0f),
		glm::vec3(1.0f, -1.0f,  1.0f),
		glm::vec3(-1.0f, -1.0f,  1.0f),

		glm::vec3(-1.0f,  1.0f, -1.0f),
		glm::vec3(1.0f,  1.0f, -1.0f),
		glm::vec3(1.0f,  1.0f,  1.0f),
		glm::vec3(1.0f,  1.0f,  1.0f),
		glm::vec3(-1.0f,  1.0f,  1.0f),
		glm::vec3(-1.0f,  1.0f, -1.0f),

		glm::vec3(-1.0f, -1.0f, -1.0f),
		glm::vec3(-1.0f, -1.0f,  1.0f),
		glm::vec3(1.0f, -1.0f, -1.0f),
		glm::vec3(1.0f, -1.0f, -1.0f),
		glm::vec3(-1.0f, -1.0f,  1.0f),
		glm::vec3(1.0f, -1.0f,  1.0f)
	};
	glGenVertexArrays(1, &this->VAO);
	glBindVertexArray(this->VAO);

	glGenBuffers(1, &this->VBO);
	glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), &vertices[0], GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	Shader* c = new Shader(vertex_path, fragment_path);
	this->Shader_ID = c->shader;

	//std::cout << "Shader id: " << this->Shader << std::endl;

	// Load texture

	//this->Texture = textureID;
}

void CubeMap::Destroy() {
	glDeleteProgram(this->Shader_ID);
	glDeleteBuffers(1, &this->VBO);
	glDeleteVertexArrays(1, &this->VAO);
	texture->Delete();
}

void CubeMap::Draw(Camera* camera) {
	glDepthMask(GL_FALSE);
	glUseProgram(this->Shader_ID);

	unsigned int size = 50;

	GLuint ViewID = glGetUniformLocation(this->Shader_ID, "View");
	GLuint ProjectionID = glGetUniformLocation(this->Shader_ID, "Projection");
	GLuint ScaleID = glGetUniformLocation(this->Shader_ID, "Scale");

	glm::mat4 ProjectionMatrix = glm::perspective(45.f, (GLfloat)camera->getResolution().x / (GLfloat)camera->getResolution().y, 0.1f, 1000.0f);
	glm::mat4 mat = camera->GetViewMatrix();
	glm::mat4 ViewMatrix = mat;
	glm::mat4 ModelMatrix = glm::translate(glm::mat4(1.f), camera->getPosition());
	glm::mat4 Scale = glm::scale(glm::mat4(1.f), glm::vec3(size));

	glUniformMatrix4fv(ViewID, 1, GL_FALSE, glm::value_ptr(ViewMatrix));
	glUniformMatrix4fv(ProjectionID, 1, GL_FALSE, glm::value_ptr(ProjectionMatrix));

	glm::mat4 res = ModelMatrix * Scale;

	glUniformMatrix4fv(ScaleID, 1, GL_FALSE, glm::value_ptr(res));

	glUniform1i(glGetUniformLocation(Shader_ID, "skybox"), 0);
	glBindTexture(GL_TEXTURE_2D, this->texture->getID());

	glBindVertexArray(this->VAO);
	glEnableVertexAttribArray(0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, this->texture->getID());
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glDepthMask(GL_TRUE);
}