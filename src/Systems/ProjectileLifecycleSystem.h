#ifndef PROJECTILELIFECYCLESYSTEM_H
#define PROJECTILELIFECYCLESYSTEM_H

#include <SDL2/SDL.h>

#include "../ECS/ECS.h"
#include "../Components/ProjectileComponent.h"

class ProjectileLifecycleSystem : public System {

public:
	ProjectileLifecycleSystem() {
		requireComponent<ProjectileComponent>();
	}

	void update() {
		for (auto entity : getEntities()) {
			const auto projectile = entity.getComponent<ProjectileComponent>();

			if (SDL_GetTicks() - projectile.startTime > projectile.duration) {
				entity.kill();
			}
		}
	}

};

#endif