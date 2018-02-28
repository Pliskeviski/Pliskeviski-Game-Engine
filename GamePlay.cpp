#include "GamePlay.h"

GamePlay::GamePlay() {

	scene = new Scene();
	scene->setResolution(1366, 768);
	
	std::function<void(void)> update_cb = std::bind(&GamePlay::Update, this);
	scene->UpdateCallback(update_cb);
	if (!scene->Init()) 
		return;
	// Cubemap

	scene_shader = new Shader("res/Shaders/shader.vertexshader", "res/Shaders/shader.fragmentshader");
	scene->bindShader(scene_shader);

	std::vector<std::string> faces{
		"res/skybox/red/bkg1_right1.png",
		"res/skybox/red/bkg1_left2.png",
		"res/skybox/red/bkg1_top3.png",
		"res/skybox/red/bkg1_bottom4.png",
		"res/skybox/red/bkg1_front5.png",
		"res/skybox/red/bkg1_back6.png"
	};

	Texture* t_cubemap = new Texture(faces, TYPE_CUBEMAP);
	CubeMap* cubeMap = new CubeMap();
	cubeMap->bindTexture(t_cubemap);
	scene->addCubeMap(cubeMap);

	//glPolygonMode(GL_FRONT, GL_LINE);
	
	Texture* diff = new Texture("res/PBR/rustediron2_basecolor.png", TYPE_DIFFUSE);
	Texture* normal = new Texture("res/PBR/rustediron2_normal.png", TYPE_NORMAL);
	Texture* rough = new Texture("res/PBR/rustediron2_roughness.png", TYPE_ROUGHNESS);
	Texture* metall = new Texture("res/PBR/rustediron2_metallic.png", TYPE_METALLIC);
	//int nrRows = 7;
	//int nrColumns = 7;
	//float spacing = 2.5;
	//for (unsigned int row = 0; row < nrRows; ++row)
	//{
	//	for (unsigned int col = 0; col < nrColumns; ++col)
	//	{
	//		Object* test = new Object("res/PBR/model.obj");
	//		scene->addObject(test);
	//		test->addTexture(diff);
	//		test->addTexture(normal);
	//		test->addTexture(rough);
	//		test->addTexture(metall);
	//		test->setPosition(glm::vec3((float)(col - (nrColumns / 2)) * spacing, (float)(row - (nrRows / 2)) * spacing, 0.0f));
	//	}
	//}

	cube = new Object("res/PBR/model.obj");
	scene->addObject(cube);
	cube->addTexture(diff);
	cube->addTexture(normal);
	cube->addTexture(rough);
	cube->addTexture(metall);
	cube->setPosition(glm::vec3(2.f, 2.f, 2.f));

	// Wood floor
	Texture* floor_texture =		new Texture("res/PBR/HIRES/WoodPlanksWorn19_COL_VAR1_HIRES.jpg", TYPE_DIFFUSE);
	Texture* floor_texture_NORMAL = new Texture("res/PBR/HIRES/WoodPlanksWorn19_NRM_HIRES.jpg", TYPE_NORMAL);
	Texture* floor_texture_REFL =   new Texture("res/PBR/HIRES/WoodPlanksWorn19_GLOSS_HIRES.jpg", TYPE_METALLIC);
	Texture* floor_texture_DISP =   new Texture("res/PBR/HIRES/WoodPlanksWorn19_REFL_HIRES.jpg", TYPE_ROUGHNESS);

	Object* floor = new Object("res/Models/b_floor.obj");
	floor->addTexture(floor_texture);
	floor->addTexture(floor_texture_NORMAL);
	floor->addTexture(floor_texture_REFL);
	floor->addTexture(floor_texture_DISP);
	scene->addObject(floor);

	floor->setPosition(glm::vec3(floor->getPosition().x, floor->getPosition().y - 3, floor->getPosition().z + 8));
	
	// tiles wall
	Texture* wall_texture_DF = new Texture("res/Wall/TilesOnyxOpaloBlack001_COL_4K.jpg", TYPE_DIFFUSE);
	Texture* wall_texture_NR = new Texture("res/Wall/TilesOnyxOpaloBlack001_NRM_4K.jpg", TYPE_NORMAL);
	Texture* wall_texture_MT = new Texture("res/Wall/TilesOnyxOpaloBlack001_GLOSS_4K.jpg", TYPE_METALLIC);
	Texture* wall_texture_RG = new Texture("res/Wall/TilesOnyxOpaloBlack001_REFL_4K.jpg", TYPE_ROUGHNESS);

	Object* wall = new Object("res/Wall/b_wall.obj");
	wall->addTexture(wall_texture_DF);
	wall->addTexture(wall_texture_NR);
	wall->addTexture(wall_texture_MT);
	wall->addTexture(wall_texture_RG);
	scene->addObject(wall);

	wall->setRotationX(90);
	wall->setPosition(glm::vec3(0, 4, 0));

	// gun
	Texture* Gun_texture_DF_1 = new Texture("res/Mat49/02___Default_albedo.jpeg", TYPE_DIFFUSE);
	Texture* Gun_texture_NR_1 = new Texture("res/Mat49/02___Default_normal.jpeg", TYPE_NORMAL);
	Texture* Gun_texture_MT_2 = new Texture("res/Mat49/02___Default_metallic.jpeg", TYPE_METALLIC);
	Texture* Gun_texture_RG_1 = new Texture("res/Mat49/02___Default_roughness.jpeg", TYPE_ROUGHNESS);
	Texture* Gun_texture_AO_1 = new Texture("res/Mat49/02___Default_AO.jpeg", TYPE_AO);

	Gun = new Object("res/Mat49/Model.dae");
	Gun->addTexture(Gun_texture_DF_1);
	Gun->addTexture(Gun_texture_NR_1);
	Gun->addTexture(Gun_texture_AO_1);
	Gun->addTexture(Gun_texture_RG_1);
	Gun->addTexture(Gun_texture_AO_1);

	Gun->setPosition(glm::vec3(.1f, 1.f, 8.f));
	//Gun->setScale(.025f);
	scene->addObject(Gun);
	
	Texture* ball_df = new Texture("res/Models/2k_x_2k.png", TYPE_DIFFUSE);
	Texture* ball_mt = new Texture("res/Models/2k_x_2k.png", TYPE_METALLIC);
	Object* ball = new Object("res/Models/ball.obj");
	ball->addTexture(ball_df);
	ball->addTexture(ball_mt);

	scene->addObject(ball);

	ball->setPositionZ(-3.f);

	light = new Light(scene_shader);
	scene->addLight(light);
	light->setPosition(glm::vec3(1.f, 1.f, 1.f));
	light->setColor(glm::vec3(1.f, 1.f, 1.f));

	light_2 = new Light(scene_shader, glm::vec3(sin(10 / 5), 0.f, 4.f), glm::vec3(7.f, sin(10 / 5), .7f));
	scene->addLight(light_2);
	light_2->setPosition(glm::vec3(sin(10 / 5), 0.f, 4.f));
	light_2->setColor(glm::vec3(7.f, sin(10 / 5), .7f));

	// TODO add (isAcive) to lighting

	scene->Start();
	scene->Delete();
}

float i = 0;

void GamePlay::Update() {
	i += .5;
	light->setIntensity(light->getIntensity() + i*10);
	light->setPosition(glm::vec3(1.f, sin(i/10), 1.f));
	Gun->setRotationY(i);
	cube->setRotationX(i);
	cube->setRotationY(i + .3f);
	cube->setRotationZ(i * .4f);

	light_2->setPosition(glm::vec3(cos(i/10), cos(i / 10), 2.f));
	light_2->setColor(glm::vec3(cos((i + 1) / 17), sin((i + 0.5f) / 15), cos(sin((i + 2) / 20))));
	light_2->setIntensity(1000 * i);

	if (i == 1000) {
		Light* l = new Light(scene_shader, glm::vec3(sin(i / 5), 0.f, 4.f), glm::vec3(7.f, sin(i / 5), .7f));
		scene->addLight(l);
		l->setPosition(glm::vec3(sin(i/5), 0.f, 4.f));
		l->setColor(glm::vec3(7.f, sin(i / 5), .7f));
		cube->setActive(false);
	}
}
