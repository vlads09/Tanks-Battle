#pragma once

#include "components/simple_scene.h"
#include "lab_m1/Tema2/lab_camera.h"


namespace m1
{
	class Ammo : public gfxc::SimpleScene {
	public:
		Ammo::Ammo()
		{
		}


		Ammo::~Ammo()
		{
		}

	public:
		glm::vec3 initialPosition;
		glm::vec3 position;
		bool shoot = true;
		glm::mat4 matrix;
	};
}

