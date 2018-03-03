#pragma once



#include <vector>

#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>

#include "Texture.h"

#include "../Shader/Shader.h"
#include "../Tools/PlisReader.h"

struct subMesh {
	std::vector<glm::vec3> Vertices;
	std::vector<glm::vec3> Normals;
	std::vector<glm::vec2> UVs;
	
	std::vector<glm::vec3> Tangents;
	std::vector<glm::vec3> BiTangents;

	std::vector<unsigned short> Indices;

	std::vector<Texture*> m_texture; //

	unsigned int drawSize;
	GLuint VAO;
	std::vector<GLuint> VBOs;
	GLuint EBO;

	inline void Destroy() {
		for (unsigned int i = 0; i < m_texture.size(); i++){
			m_texture[i]->Delete();
			//delete m_texture[i];
		}
	}

	inline void S_Draw(glm::mat4 ViewMatrix_, Shader* Shader, glm::vec3 position, glm::vec3 rotation, glm::vec3 scale) {
		Shader->Use();
		glBindVertexArray(this->VAO);
		for (unsigned int j = 0; j < this->VBOs.size(); j++) {
			glEnableVertexAttribArray(j);
		}

		GLuint ViewID = glGetUniformLocation(Shader->shader, "view");
		GLuint ProjectionID = glGetUniformLocation(Shader->shader, "projection");
		GLuint ModelID = glGetUniformLocation(Shader->shader, "model");

		glm::mat4 ProjectionMatrix = glm::perspective(45.f, (GLfloat)1366 / (GLfloat)768, 0.1f, 1000.0f);
		glm::mat4 ViewMatrix = ViewMatrix_;
		glm::mat4 ModelMatrix = glm::translate(glm::mat4(1.f), position);
		glm::mat4 ModelRX = glm::rotate(glm::mat4(1.f), rotation.x, glm::vec3(1.0f, .0f, .0f));
		glm::mat4 ModelRY = glm::rotate(glm::mat4(1.f), rotation.y, glm::vec3(.0f, 1.0f, .0f));
		glm::mat4 ModelRZ = glm::rotate(glm::mat4(1.f), rotation.z, glm::vec3(.0f, .0f, 1.0f));
		glm::mat4 Model3 = glm::scale(glm::mat4(1.f), scale);
		glm::mat4 MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;

		glm::mat4 ModelRotation = ModelRX * ModelRY * ModelRZ;

		ModelMatrix *= ModelRotation * Model3;

		glUniformMatrix4fv(ModelID, 1, GL_FALSE, glm::value_ptr(ModelMatrix));
		glUniformMatrix4fv(ViewID, 1, GL_FALSE, glm::value_ptr(ViewMatrix));
		glUniformMatrix4fv(ProjectionID, 1, GL_FALSE, glm::value_ptr(ProjectionMatrix));

		if (!m_texture.empty()) {
			for (unsigned int i = 0; i < m_texture.size(); i++) {
				glActiveTexture(GL_TEXTURE0 + i);
				std::string name;
				if (m_texture[i]->getType() == TYPE_DIFFUSE)
					name = "albedoMap";
				if (m_texture[i]->getType() == TYPE_NORMAL)
					name = "normalMap";
				if (m_texture[i]->getType() == TYPE_METALLIC)
					name = "metallicMap";
				if (m_texture[i]->getType() == TYPE_ROUGHNESS)
					name = "roughnessMap";
				if (m_texture[i]->getType() == TYPE_AO)
					name = "aoMap";

				glUniform1i(glGetUniformLocation(Shader->shader, name.c_str()), i);
				glBindTexture(GL_TEXTURE_2D, m_texture[i]->getID());
			}
		}
		glDrawElements(GL_TRIANGLES, this->drawSize, GL_UNSIGNED_SHORT, (void*)0);
		for (int j = 0; j < this->VBOs.size(); j++) {
			glDisableVertexAttribArray(j);
		}
		glBindTexture(GL_TEXTURE_2D, 0);
	}
	// USE WHEN DRAWING LIGHTS
	inline void S_Draw(glm::mat4 ViewMatrix_, glm::vec3 position, glm::vec3 rotation, glm::vec3 scale, GLuint s_Shader) {
		glUseProgram(s_Shader);
		glBindVertexArray(this->VAO);
		for (unsigned int j = 0; j < this->VBOs.size(); j++) {
			glEnableVertexAttribArray(j);
		}

		GLuint ViewID = glGetUniformLocation(s_Shader, "view");
		GLuint ProjectionID = glGetUniformLocation(s_Shader, "projection");
		GLuint ModelID = glGetUniformLocation(s_Shader, "model");

		glm::mat4 ProjectionMatrix = glm::perspective(45.f, (GLfloat)1366 / (GLfloat)768, 0.1f, 1000.0f);
		glm::mat4 ViewMatrix = ViewMatrix_;
		glm::mat4 ModelMatrix = glm::translate(glm::mat4(1.f), position);
		glm::mat4 ModelRX = glm::rotate(glm::mat4(1.f), rotation.x, glm::vec3(1.0f, .0f, .0f));
		glm::mat4 ModelRY = glm::rotate(glm::mat4(1.f), rotation.y, glm::vec3(.0f, 1.0f, .0f));
		glm::mat4 ModelRZ = glm::rotate(glm::mat4(1.f), rotation.z, glm::vec3(.0f, .0f, 1.0f));
		glm::mat4 Model3 = glm::scale(glm::mat4(1.f), scale);
		glm::mat4 MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;

		glm::mat4 ModelRotation = ModelRX * ModelRY * ModelRZ;

		ModelMatrix *= ModelRotation * Model3;

		glUniformMatrix4fv(ModelID, 1, GL_FALSE, glm::value_ptr(ModelMatrix));
		glUniformMatrix4fv(ViewID, 1, GL_FALSE, glm::value_ptr(ViewMatrix));
		glUniformMatrix4fv(ProjectionID, 1, GL_FALSE, glm::value_ptr(ProjectionMatrix));

		

		glDrawElements(GL_TRIANGLES, this->drawSize, GL_UNSIGNED_SHORT, (void*)0);
		for (int j = 0; j < this->VBOs.size(); j++) {
			glDisableVertexAttribArray(j);
		}
	}

};

struct Mesh {
	const char* m_path;
	unsigned int m_id;
	std::vector<subMesh*> meshes;
	glm::vec3 m_position;
	glm::vec3 m_scale;
	glm::vec3 m_rotation;
	inline void ClearTexture(); // TODO
};

class Object {
public:
	Object(const char* path);
	void Draw(Shader* s_Shader, glm::mat4 ViewMatrix_);

	void addTexture(Texture* texture);
	void Destroy();

	inline void setScale(float scale) { this->t_mesh->m_scale = glm::vec3(scale); } // TODO scaleX, scaleY, scaleZ
	
	inline void setPosition(glm::vec3 pos) { this->t_mesh->m_position = pos; }
	inline void setPositionX(float x) { this->t_mesh->m_position.x = x; }
	inline void setPositionY(float y) { this->t_mesh->m_position.y = y; }
	inline void setPositionZ(float z) { this->t_mesh->m_position.z = z; }

	inline void setRotationX(float angle) { t_mesh->m_rotation.x = (glm::radians(angle)); };
	inline void setRotationY(float angle) { t_mesh->m_rotation.y = (glm::radians(angle)); };
	inline void setRotationZ(float angle) { t_mesh->m_rotation.z = (glm::radians(angle)); };
		
	inline void setActive(bool active) { this->Active = active; }
	inline bool isActive() { return this->Active; }

	inline glm::vec3 getPosition() { return this->t_mesh->m_position; }
	inline glm::vec3 getRotation() { return this->t_mesh->m_rotation; }
	inline glm::vec3    getScale() {	return this->t_mesh->m_scale; }
	inline std::vector<subMesh*> getMeshes() { return t_mesh->meshes; }
private:
	GLuint t_shader; // Only use for light
	Mesh* t_mesh;
	void CreateBuffers(subMesh* s_mesh);
	bool Active;
	GLuint genVBO(subMesh* m_mesh, GLsizeiptr size, GLint dataSize, const void* data, GLenum usage);
};

