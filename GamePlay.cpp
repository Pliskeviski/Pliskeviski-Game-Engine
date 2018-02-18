#include "GamePlay.h"

void GamePlay::Start() {
	Scene* scene = new Scene();
	scene->setResolution(glm::vec2(1366, 768));
	scene->Init();

	GLuint shader = Shader::CreateShader("res/shaders/shader.vertexshader", "res/shaders/shader.fragmentshader");

	Texture texAK("res/Objects/AK_diffuse.jpg");
	Object* ak = new Object("res/Objects/AK.plge", 0);
	ak->addTexture(texAK);
	ak->bindShader(shader);
	scene->addObject(ak);
	ak->setScale(.025f);
	ak->setRotationY(1);

	Texture texSniper("res/Objects/Sniper_KSR_29_Col.jpg");
	Object* sniper = new Object("res/Objects/SNIPER.plge", 0);
	sniper->addTexture(texSniper);
	sniper->bindShader(shader);
	scene->addObject(sniper);
	sniper->setPosition(glm::vec3(5.f, 1.f, 2.f));
	//
	//Texture texBall("res/Objects/NBA_BASKETBALL_DIFFUSE.tga");
	//Object* ball = new Object("res/Objects/NBA BASKETBALL.obj", glm::vec3(1.f, 1.f, 4.f), 0);
	//ball->addTexture(texBall);
	//ball->bindShader(shader);
	//scene->addObject(ball);
	//ball->setScale(.025f);
	float i = 1;
	while (glfwGetKey(scene->getWindow(), GLFW_KEY_ESCAPE) != GLFW_PRESS && glfwWindowShouldClose(scene->getWindow()) == 0) {
		computeMatricesFromInputs();
		// Clear the screen
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		scene->draw(shader);

		i += 0.02f;
		ak->setRotationY(i);
		sniper->setRotationY(i+0.2f);

		glfwSwapBuffers(scene->getWindow());
		glfwPollEvents();
	}
	scene->Destroy();
	glDeleteProgram(shader);
	glfwTerminate();

}

