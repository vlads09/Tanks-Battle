#pragma once

#include "components/simple_scene.h"
#include "lab_m1/Tema2/lab_camera.h"
#include "lab_m1/Tema2/Ammo.h"

namespace m1
{
	class Tank : public gfxc::SimpleScene {
	public:
		Tank::Tank()
		{
		}


		Tank::~Tank()
		{
		}

	public:
		float speedAll = 2;
		float speedRotTurret = 2;
		float speedRotCannon = 2;
		glm::vec3 body_pos = glm::vec3(0, 0.5, 0);
		glm::vec3 cannon_pos = glm::vec3(0, -0.3, 2.2);
		glm::vec3 turret_pos = glm::vec3(0, 1.4, 0);
		glm::vec3 scale = glm::vec3(0.5, 0.5, 0.5);
		float rotate_body = RADIANS(0);
		float rotate_turret = RADIANS(0);
		float up_down_cannon = RADIANS(0);
		glm::vec3 forward = glm::vec3(0, 0, -1);
		glm::vec3 right = glm::vec3(0, 0, 1);
		std::vector<Ammo> ammos;
		int HP = 100;
	};
}

