#ifndef PROJECTILLEEMITTERCOMPONENT_H
#define PROJECTILLEEMITTERCOMPONENT_H

#include <glm/glm.hpp>
#include <SDL2/SDL.h>

#include "../ECS/ECS.h"

struct ProjectileEmitterComponent : IComponent {

	glm::vec2 velocity;
	int repeatFrequency;
	int duration;
	int hitPercentDamage;
	bool isFriendly;
	int lastEmissionTime;

	ProjectileEmitterComponent(glm::vec2 velocity = glm::vec2(0.0),
		int repeatFrequency = 0,
		int duration = 10000,
		int hitPercentDamage = 10,
		bool isFriendly = false) {

		this->velocity = velocity;
		this->repeatFrequency = repeatFrequency;
		this->duration = duration;
		this->hitPercentDamage = hitPercentDamage;
		this->isFriendly = isFriendly;
		this->lastEmissionTime = SDL_GetTicks();
	}

};

#endif