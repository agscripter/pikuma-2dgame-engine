#ifndef RENDERHEALTHBARSYSTEM_H
#define RENDERHEALTHBARSYSTEM_H

#include <SDL2/SDL.h>

#include "../ECS/ECS.h"
#include "../AssetStore/AssetStore.h"

#include "../Components/HealthComponent.h"
#include "../Components/TransformComponent.h"
#include "../Components/SpriteComponent.h"

class RenderHealthBarSystem: public System {
public:
	RenderHealthBarSystem() {
		requireComponent<TransformComponent>();
		requireComponent<SpriteComponent>();
		requireComponent<HealthComponent>();
	}

	void update(SDL_Renderer* renderer, std::unique_ptr<AssetStore>& assetStore,
		const SDL_Rect& camera) {

		for (auto entity: getEntities()) {
			const auto transform = entity.getComponent<TransformComponent>();
			const auto sprite = entity.getComponent<SpriteComponent>();
			const auto health = entity.getComponent<HealthComponent>();

			SDL_Color healthBarColor = {255, 255, 255};

			if (health.healthPercentage >= 0 && health.healthPercentage < 40) {
				healthBarColor = {255, 0, 0};
			}

			if (health.healthPercentage >= 40 && health.healthPercentage < 80) {
				healthBarColor = {255, 255, 0};
			}

			if (health.healthPercentage >= 80 && health.healthPercentage <= 100) {
				healthBarColor = {0, 255, 0};
			}

			// Health bar
			int healthBarWidth = 15;
			int healthBarHeight = 3;

			double healthBarPosX = (transform.position.x + (sprite.width * transform.scale.x)) - camera.x;
			double healthBarPosY = (transform.position.y) - camera.y;

			SDL_Rect healthBarRectangle = {
				static_cast<int>(healthBarPosX),
				static_cast<int>(healthBarPosY),
				static_cast<int>(healthBarWidth * (health.healthPercentage / 100.0)),
				static_cast<int>(healthBarHeight)
			};

			SDL_SetRenderDrawColor(renderer, healthBarColor.r, healthBarColor.g, healthBarColor.b, 255);
			SDL_RenderFillRect(renderer, &healthBarRectangle);

			// Health Percentage
			std::string healthText = std::to_string(health.healthPercentage);
			TTF_Font* font = assetStore->getFont("pico8-font-5");

			SDL_Surface* surface = TTF_RenderText_Blended(
				font, 
				healthText.c_str(), 
				healthBarColor
			);

			SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
			SDL_FreeSurface(surface);

			int labelWidth = 0;
			int labelHeight = 0;

			SDL_QueryTexture(texture, NULL, NULL, &labelWidth, &labelHeight);

			SDL_Rect destinationRect = {
				static_cast<int>(healthBarPosX),
				static_cast<int>(healthBarPosY) + 5,
				labelWidth,
				labelHeight
			};

			SDL_RenderCopy(renderer, texture, NULL, &destinationRect);
			SDL_DestroyTexture(texture);
		}
	}

};

#endif