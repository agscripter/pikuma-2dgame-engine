#ifndef ANIMATIONCOMPONENT_H
#define ANIMATIONCOMPONENT_H

#include <SDL2/SDL.h>
#include "../ECS/ECS.h"

struct AnimationComponent : IComponent {

	int numFrames;
	int currentFrame;
	int frameSpeedRate;
	bool shouldLoop;
	int startTime;

	AnimationComponent(int numFrames = 1, int frameSpeedRate = 1, bool shouldLoop = true) {
		this->numFrames = numFrames;
		this->currentFrame = 1;
		this->frameSpeedRate = frameSpeedRate;
		this->shouldLoop = shouldLoop;
		this->startTime = SDL_GetTicks();
	}

};


#endif