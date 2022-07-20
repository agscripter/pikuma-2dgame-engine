#ifndef HEALTHCOMPONENT_H
#define HEALTHCOMPONENT_H

#include "../ECS/ECS.h"

struct HealthComponent : IComponent {

	int healthPercentage;

	HealthComponent(int healthPercentage = 0) {
		this->healthPercentage = healthPercentage;
	}

};

#endif