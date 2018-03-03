#include "Scene.h"

GLFWwindow* window;

// Camera
GLuint WIDTH = 800, HEIGHT = 600; // Default camera resolution
Camera* camera = new Camera(glm::vec3(0.0f, 0.0f, 3.0f));
GLfloat lastX = WIDTH / 2.0;
GLfloat lastY = HEIGHT / 2.0;
bool keys[1024];
bool firstMouse = true;
GLfloat deltaTime = 0.0f;
GLfloat lastFrame = 0.0f;
// end camera

#include "Camera_Callbacks.hpp"

Scene::Scene() {
	this->t_window = window;
	this->resolution.x = WIDTH;
	this->resolution.y = HEIGHT;
}

int Scene::Init() {
	if (!glfwInit()) {
		std::cerr << "Scene: Failed to initialize glfw" << std::endl;
		getchar();
		return -1;
	}

	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	window = glfwCreateWindow(resolution.x, resolution.y, "Pliskeviski Engine", NULL, NULL);
	if (window == NULL) {
		std::cerr << "Scene: Failed to create a glfw window" << std::endl;
		getchar();
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	glewExperimental = true;
	if (glewInit() != GLEW_OK) {
		std::cerr << "Scene: Failed to initialize glew" << std::endl;
		getchar();
		glfwTerminate();
		return -1;
	}

	if (update_cb == NULL) {
		std::cerr << "Scene: Update callback not found." << std::endl;
		getchar();
		glfwTerminate();
		return -1;
	}

	// Camera callbacks

	glfwSetKeyCallback(window, KeyCallback);
	glfwSetCursorPosCallback(window, MouseCallback);
	glfwSetScrollCallback(window, ScrollCallback);

	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
	// Hide the mouse
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	glfwPollEvents();
	glfwSetCursorPos(window, resolution.x / 2, resolution.y / 2);

	glClearColor(0.7f, 0.7f, 0.7f, 0.0f);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glEnable(GL_CULL_FACE);

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);

	camera->setResolution(this->resolution);

	glfwSwapInterval(1); // Vsync | 0 = off, 1 = on

	std::cout << "Scene initialized." << std::endl;

	return 1;
}

void Scene::Start() {
	while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS && glfwWindowShouldClose(window) == 0) {
		GLfloat currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
		// Check and call events
		glfwPollEvents();
		DoMovement();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Update callback funcion
		Draw();
		update_cb();

		glfwSwapBuffers(window);
	}
}

void Scene::Delete() { // TODO
	if (!c_map.empty())
		c_map[0]->Destroy();
	
	for (unsigned int i = 0; i < s_obj.size(); i++) {
		s_obj[i]->Destroy();
		delete s_obj[i];
	}

	for (unsigned int i = 0; i < s_light.size(); i++) {
		delete s_light[i];
	}

	Light::Delete();
	delete this;
	glfwTerminate();
}

void Scene::Draw() {
	// CubeMap
	if (!c_map.empty()) {
		if(c_map[0]->isActive())
			c_map[0]->Draw(camera);
	}

	// Lights
	for (unsigned int i = 0; i < s_light.size(); i++) {
		if(s_light[i]->isActive())
			s_light[i]->Draw(camera->GetViewMatrix());
	}

	// Objects
	for (unsigned int i = 0; i < s_obj.size(); i++) {
		if (s_obj[i]->isActive()) {
			Shader::setVec3(this->scene_shader, "camPos", camera->getPosition().x, camera->getPosition().y, camera->getPosition().z);
			s_obj[i]->Draw(this->scene_shader, camera->GetViewMatrix());
		}
	}

}

Camera* getCamera() { return camera; }