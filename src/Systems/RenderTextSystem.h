#ifndef RENDERTEXTSYSTEM_H
#define RENDERTEXTSYSTEM_H

#include <SDL2/SDL.h>

#include "../ECS/ECS.h"
#include "../AssetStore/AssetStore.h"
#include "../Components/TextLabelComponent.h"

class RenderTextSystem : public System {
public:
	RenderTextSystem() {
		requireComponent<TextLabelComponent>();
	}

	void update(SDL_Renderer* renderer, std::unique_ptr<AssetStore>& assetStore,
		const SDL_Rect& camera) {
		for (auto entity: getEntities()) {
			const auto textLabelComponent = entity.getComponent<TextLabelComponent>();

			TTF_Font* font = assetStore->getFont(textLabelComponent.assetId);

			SDL_Surface* surface = TTF_RenderText_Blended(
				font, 
				textLabelComponent.text.c_str(), 
				textLabelComponent.color
			);

			SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
			SDL_FreeSurface(surface);

			int labelWidth = 0;
			int labelHeight = 0;

			SDL_QueryTexture(texture, NULL, NULL, &labelWidth, &labelHeight);

			SDL_Rect destinationRect = {
				static_cast<int>(textLabelComponent.position.x - (textLabelComponent.isFixed ? 0 : camera.x)),
				static_cast<int>(textLabelComponent.position.y - (textLabelComponent.isFixed ? 0 : camera.y)),
				labelWidth,
				labelHeight
			};

			SDL_RenderCopy(renderer, texture, NULL, &destinationRect);
			SDL_DestroyTexture(texture);
		}
	}

};


#endif