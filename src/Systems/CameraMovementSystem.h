#ifndef CAMERAMOVEMENTSYSTEM_H
#define CAMERAMOVEMENTSYSTEM_H

#include <SDL2/SDL.h>

#include "../ECS/ECS.h"
#include "../Components/CameraFollowComponent.h"
#include "../Components/TransformComponent.h"

class CameraMovementSystem : public System {

public:
	CameraMovementSystem() {
		requireComponent<CameraFollowComponent>();
		requireComponent<TransformComponent>();
	}

	void update(SDL_Rect& camera) {
		for (auto entity : getEntities()) {
			auto transform = entity.getComponent<TransformComponent>();

			if (transform.position.x + (camera.w / 2) < Game::mapWidth) {
				camera.x = transform.position.x - (Game::windowWidth / 2);
			}

			if (transform.position.y + (camera.h / 2) < Game::mapHeight) {
				camera.y = transform.position.y - (Game::windowHeight / 2);
			}

			camera.x = camera.x < 0 ? 0 : camera.x;
			camera.y = camera.y < 0 ? 0 : camera.y;

			camera.x = camera.x > camera.w ? camera.w : camera.x;
			camera.y = camera.y > camera.h ? camera.h : camera.y;
		}
	}

};

#endif