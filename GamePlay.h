#pragma once

#include <iostream>
#include <vector>

#include "Core\Scene.h"
#include "Core\CubeMap.h"
#include "Core\Texture.h"
#include "Core\Object.h"
#include "Core\Camera.h"

class GamePlay {
public:
	GamePlay();
	void Update();
private:
	Scene* scene;
	Shader* scene_shader;

	Object* Gun;
	Object* cube;

	Light* light;
	Light* light_2;
};
