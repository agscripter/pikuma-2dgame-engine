#ifndef RENDERSYSTEM_H
#define RENDERSYSTEM_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "../ECS/ECS.h"
#include "../Logger/Logger.h"
#include "../Components/TransformComponent.h"
#include "../Components/SpriteComponent.h"
#include "../AssetStore/AssetStore.h"

class RenderSystem : public System {

public:
	RenderSystem() {
		requireComponent<TransformComponent>();
		requireComponent<SpriteComponent>();
	}

	void update(SDL_Renderer* renderer, std::unique_ptr<AssetStore>& assetStore, SDL_Rect& camera) {
		struct RenderableEntity {
			TransformComponent transformComponent;
			SpriteComponent spriteComponent;
		};

		std::vector<RenderableEntity> renderableEntities; 
		for (auto entity: getEntities()) {
			RenderableEntity renderableEntity;

			auto transform = entity.getComponent<TransformComponent>();
			auto sprite = entity.getComponent<SpriteComponent>();

			renderableEntity.transformComponent = transform;
			renderableEntity.spriteComponent = sprite;

			bool isEntityOutsideCameraView = (
				transform.position.x + (transform.scale.x * sprite.width) < camera.x ||
				transform.position.x > camera.x + camera.w ||
				transform.position.y + (transform.scale.y * sprite.height) < camera.y ||
				transform.position.y > camera.y + camera.h
			);
			
			if (isEntityOutsideCameraView && !sprite.isFixed) {
				continue;
			}

			renderableEntities.emplace_back(renderableEntity);
		}

		std::sort(renderableEntities.begin(), renderableEntities.end(), [](const RenderableEntity& a, const RenderableEntity& b) {
			return a.spriteComponent.zIndex < b.spriteComponent.zIndex;
		});

		for (auto entity: renderableEntities) {
			const auto transform = entity.transformComponent;
			const auto sprite = entity.spriteComponent;

			SDL_Texture* texture = assetStore->getTexture(sprite.assetId);

			SDL_Rect source = sprite.sourceRect;

			SDL_Rect destination = {
				static_cast<int>(transform.position.x - (sprite.isFixed ? 0 : camera.x)),
                static_cast<int>(transform.position.y - (sprite.isFixed ? 0 : camera.y)),
				static_cast<int>(sprite.width * transform.scale.x),
				static_cast<int>(sprite.height * transform.scale.y)
			};

			SDL_RenderCopyEx(
				renderer, 
				texture, 
				&source, 
				&destination,
				transform.rotation,
				NULL,
				sprite.flip
			);

			//Logger::info("Rendered entity id = " + std::to_string(entity.getId()));
		}
	}

};

#endif