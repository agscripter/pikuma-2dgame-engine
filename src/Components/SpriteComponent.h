#ifndef SPRITECOMPONENT_H
#define SPRITECOMPONENT_H

#include <string>

#include <SDL2/SDL.h>

#include "../ECS/ECS.h"

struct SpriteComponent : IComponent {
	std::string assetId;
	int width;
	int height;
	int zIndex;
	bool isFixed;
	SDL_Rect sourceRect;

	SpriteComponent(
		std::string assetId = "", 
		int zIndex = 0, 
		int width = 0, int height = 0,
		bool isFixed = false,
		int sourceRectangleX = 0, int sourceRectangleY = 0) {

		this->assetId = assetId;
		this->width = width;
		this->height = height;
		this->zIndex = zIndex;
		this->isFixed = isFixed;
		this->sourceRect = {
			sourceRectangleX, 
			sourceRectangleY,
			width,
			height
		};
	}

};

#endif