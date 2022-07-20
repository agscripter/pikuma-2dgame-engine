#ifndef ANIMATIONSYSTEM_H
#define ANIMATIONSYSTEM_H

#include <SDL2/SDL.h>

#include "../ECS/ECS.h"
#include "../Components/AnimationComponent.h"
#include "../Components/SpriteComponent.h"

class AnimationSystem : public System {

public:
	AnimationSystem() {
		requireComponent<AnimationComponent>();
		requireComponent<SpriteComponent>();
	}

	void update() {
		for (auto entity: getEntities()) {
			auto& animation = entity.getComponent<AnimationComponent>();
			auto& sprite = entity.getComponent<SpriteComponent>();

			int timePassed = SDL_GetTicks() - animation.startTime;
			animation.currentFrame = (timePassed * animation.frameSpeedRate / 1000) % animation.numFrames;
			sprite.sourceRect.x = animation.currentFrame * sprite.width;
		}
	}

};

#endif