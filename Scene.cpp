#include "Scene.h"

GLFWwindow* window;

Scene::Scene() {
	resolution.x = 800;
	resolution.y = 600;
}
int Scene::Init() {

	if (!glfwInit()) {
		std::cerr << "Failed to initialize glfw" << std::endl;
		getchar();
		return -1;
	}

	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	window = glfwCreateWindow(resolution.x, resolution.y, "Pliskeviski Engine", NULL, NULL);
	if (window == NULL) {
		std::cerr << "Failed to create a glfw window" << std::endl;
		getchar();
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	glewExperimental = true;
	if (glewInit() != GLEW_OK) {
		std::cerr << "Failed to initialize glew" << std::endl;
		getchar();
		glfwTerminate();
		return -1;
	}

	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
	// Hide the mouse
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	glfwPollEvents();
	glfwSetCursorPos(window, resolution.x / 2, resolution.y / 2);

	glClearColor(0.5f, 0.5f, 0.5f, 0.0f);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glEnable(GL_CULL_FACE);

	return 1;
}

void Scene::draw(GLuint shader) {
	GLuint ViewID = glGetUniformLocation(shader, "View");
	GLuint ProjectionID = glGetUniformLocation(shader, "Projection");
	GLuint ModelID = glGetUniformLocation(shader, "Model");
	glm::mat4 ProjectionMatrix = glm::perspective(45.f, (GLfloat)800 / (GLfloat)600, 0.1f, 1000.0f);
	glm::mat4 ViewMatrix = getViewMatrix();
	glm::mat4 ModelMatrix = glm::translate(glm::mat4(1.f), glm::vec3(.0f, .0f, .0f));
	glm::mat4 MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
	glUniformMatrix4fv(ViewID, 1, GL_FALSE, glm::value_ptr(ViewMatrix));
	glUniformMatrix4fv(ModelID, 1, GL_FALSE, glm::value_ptr(ModelMatrix));
	glUniformMatrix4fv(ProjectionID, 1, GL_FALSE, glm::value_ptr(ProjectionMatrix));
	glm::vec3 camera = getPosition();
	for (unsigned int i = 0; i < objects.size(); i++) {
		if(distance(camera.x, objects[i]->getPosition().x) < 20 && distance(camera.z, objects[i]->getPosition().z) < 20)
			objects[i]->Draw();
	}

}

void Scene::Destroy() {
	for (int i = 0; i < objects.size(); i++)
		objects[i]->Destroy();
}

float Scene::distance(float camera, float cube) {
	if (camera < 0)
		camera *= -1;
	if (cube < 0)
		cube *= -1;

	float result = camera - cube;
	if (result < 0)
		return result *= -1;

	return result;
}

GLFWwindow* Scene::getWindow() { return window; }
