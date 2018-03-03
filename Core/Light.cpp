#include "Light.h"

int lightSources = 0;

int checkLights() {
	FILE* file = fopen("res/Shaders/shader.fragmentshader", "r");
	if (!file)
		return -1;

	char buffer[255];

	while (fgets(buffer, 255, (FILE*)file)) {
		if (buffer[0] == '#' && buffer[8] == 'N') {
			int n = 0;
			fscanf(file, "#define N_LIGHTS %d", n);
			fclose(file);
			return n;
		}
	}
	fclose(file);
}
int inShaderLights = checkLights();

std::string fragmentShader = "res/Shaders/shader.fragmentshader";
const char* cubePath = "res/Light_Shape/cube.obj";

Light::Light(Shader* Scene_Shader) : Debug(true), Active(true), pos (0), Scale (.3f), intensity(1) {
	this->this_Shader = new Shader("res/Shaders/light.vertexshader", "res/Shaders/light.fragmentshader");
	this->s_shader = Scene_Shader;
	Create();
}
Light::Light(Shader* Scene_Shader, glm::vec3 pos) : Debug(true), Active(true), Scale(.3f), intensity(1) {
	this->this_Shader = new Shader("res/Shaders/light.vertexshader", "res/Shaders/light.fragmentshader");
	this->s_shader = Scene_Shader;
	this->pos = pos;
	Create();
}
Light::Light(Shader* Scene_Shader, glm::vec3 pos, glm::vec3 color) : Debug(true), Active(true), Scale(.3f), intensity(1) {
	this->this_Shader = new Shader("res/Shaders/light.vertexshader", "res/Shaders/light.fragmentshader");
	this->s_shader = Scene_Shader;
	this->color = color;
	Create();
}

void Light::Create() {
	lightSources++;
	if (inShaderLights < lightSources) {
		inShaderLights = lightSources;
		CompileShader(inShaderLights);
		Shader* newShader = new Shader("res/Shaders/shader.vertexshader", "res/Shaders/shader.fragmentshader");
		glDeleteShader(s_shader->shader);
		s_shader->shader = newShader->shader;
	}
	this->index = lightSources-1;

	glGenVertexArrays(1, &this->VAO);
	glBindVertexArray(VAO);

	color = glm::vec3(1.f, 1.f, 1.f);

	GLfloat cube_vertices[] = {
		// front
		-1.0, -1.0,  1.0,
	 	 1.0, -1.0,  1.0,
		 1.0,  1.0,  1.0,
		-1.0,  1.0,  1.0,
		// back
		-1.0, -1.0, -1.0,
		 1.0, -1.0, -1.0,
		 1.0,  1.0, -1.0,
		-1.0,  1.0, -1.0,
	};
	GLushort cube_elements[] = {
		// front
		0, 1, 2,
		2, 3, 0,
		// top
		1, 5, 6,
		6, 2, 1,
		// back
		7, 6, 5,
		5, 4, 7,
		// bottom
		4, 0, 3,
		3, 7, 4,
		// left
		4, 5, 1,
		1, 0, 4,
		// right
		3, 2, 6,
		6, 7, 3,
	};

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cube_vertices), &cube_vertices[0], GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

	// EBO
	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cube_elements), &cube_elements[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

	drawSize = sizeof(cube_elements);

	
	setColor(this->color);
}

void Light::Draw(glm::mat4 viewMatrix) {

	s_shader->Use();
	Shader::setVec3(s_shader, "lightPositions[" + std::to_string(this->index) + "]", this->pos.x, this->pos.y, this->pos.z);
	Shader::setVec3(s_shader, "lightColors[" + std::to_string(this->index) + "]", this->color.x, this->color.y, this->color.z);

	if (!Debug)
		return;
	this_Shader->Use();
	glBindVertexArray(this->VAO);
	glEnableVertexAttribArray(0);

	GLuint ViewID = glGetUniformLocation(this_Shader->shader, "view");
	GLuint ProjectionID = glGetUniformLocation(this_Shader->shader, "projection");
	GLuint ModelID = glGetUniformLocation(this_Shader->shader, "model");

	glm::mat4 ProjectionMatrix = glm::perspective(45.f, (GLfloat)1366 / (GLfloat)768, 0.1f, 1000.0f);
	glm::mat4 ViewMatrix = viewMatrix;
	glm::mat4 ModelMatrix = glm::translate(glm::mat4(1.f), this->pos);
	glm::mat4 Model3 = glm::scale(glm::mat4(1.f), glm::vec3(Scale));
	glm::mat4 MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;

	ModelMatrix *=  Model3;

	glUniformMatrix4fv(ModelID, 1, GL_FALSE, glm::value_ptr(ModelMatrix));
	glUniformMatrix4fv(ViewID, 1, GL_FALSE, glm::value_ptr(ViewMatrix));
	glUniformMatrix4fv(ProjectionID, 1, GL_FALSE, glm::value_ptr(ProjectionMatrix));

	glDrawElements(GL_TRIANGLES, this->drawSize, GL_UNSIGNED_SHORT, (void*)0);
}

void Light::setColor(glm::vec3 color) {
	this->color = color;
	Shader::setVec3(this_Shader, "color", this->color.x, this->color.y, this->color.x);
}

int Light::CompileShader(int v) {
	FILE* file = fopen("res/Shaders/shader.fragmentshader", "r+");
	if (!file)
		return -1;

	FILE* temp = fopen("res/Shaders/tmp.tmp", "w+");

	char buffer[255];

	while (fgets(buffer, 255, (FILE*)file)) {
		if (buffer[0] == '#' && buffer[8] == 'N') {
			buffer[17] = std::to_string(v)[0];
		}
		fprintf(temp, buffer);

	}
	fclose(file);
	fclose(temp);

	// Deleting old .fragmentshader
	if (remove("res/Shaders/shader.fragmentshader") != 0)
		std::cout << "Failed to delete the old shader." << std::endl;

	// Rename new file (temp)
	if (std::rename("res/Shaders/tmp.tmp", "res/Shaders/shader.fragmentshader") != 0)
		std::cout << "Failed to rename the new shader." << std::endl;
}

void Light::Delete(){
	FILE* file = fopen("res/Shaders/shader.fragmentshader", "r+");
	if (!file)
		return;

	FILE* temp = fopen("res/Shaders/tmp.tmp", "w+");

	char buffer[255];

	while (fgets(buffer, 255, (FILE*)file)) {
		if (buffer[0] == '#' && buffer[8] == 'N') {
			buffer[17] = std::to_string(1)[0];
		}
		fprintf(temp, buffer);

	}
	fclose(file);
	fclose(temp);

	// Deleting old .fragmentshader
	if (remove("res/Shaders/shader.fragmentshader") != 0)
		std::cout << "Failed to delete the old shader." << std::endl;

	// Rename new file (temp)
	if (std::rename("res/Shaders/tmp.tmp", "res/Shaders/shader.fragmentshader") != 0)
		std::cout << "Failed to rename the new shader." << std::endl;
}