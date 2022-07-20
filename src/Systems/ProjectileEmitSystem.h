#ifndef PROJECTILLEEMITSYSTEM_H
#define PROJECTILLEEMITSYSTEM_H

#include <SDL2/SDL.h>

#include "../EventBus/EventBus.h"
#include "../Events/KeyPressedEvent.h"
#include "../ECS/ECS.h"
#include "../Components/ProjectileEmitterComponent.h"
#include "../Components/TransformComponent.h"
#include "../Components/RigidBodyComponent.h"
#include "../Components/SpriteComponent.h"
#include "../Components/BoxColliderComponent.h"
#include "../Components/ProjectileComponent.h"

class ProjectileEmitSystem : public System { 

public:
	ProjectileEmitSystem() {
		requireComponent<ProjectileEmitterComponent>();
		requireComponent<TransformComponent>();
	}

	void subscribeToEvents(std::unique_ptr<EventBus>& eventBus) {
		eventBus->subscribeToEvent<KeyPressedEvent>(this, &ProjectileEmitSystem::onKeyPressed);
	}

	void onKeyPressed(KeyPressedEvent& event) {
		if (event.symbol == SDLK_SPACE) {
			Logger::info("Shooting");
			for (auto entity : getEntities()) {
				if (entity.hasComponent<CameraFollowComponent>()) {
					const auto projectileEmitter = entity.getComponent<ProjectileEmitterComponent>();
					const auto transform = entity.getComponent<TransformComponent>();
					const auto rigidbody = entity.getComponent<RigidBodyComponent>();

					glm::vec2 projectilePosition = transform.position;
					if (entity.hasComponent<SpriteComponent>()) {
						const auto sprite = entity.getComponent<SpriteComponent>();
						projectilePosition.x += transform.scale.x * sprite.width / 2;
						projectilePosition.y += transform.scale.y * sprite.height / 2;
					}

					glm::vec2 projectileVelocity = projectileEmitter.velocity;

					int directionX = 0;
					int directionY = 0;

					if (rigidbody.velocity.x > 0) directionX = +1;
					if (rigidbody.velocity.x < 0) directionX = -1;
					if (rigidbody.velocity.y > 0) directionY = +1;
					if (rigidbody.velocity.y < 0) directionY = -1;

					projectileVelocity.x = projectileEmitter.velocity.x * directionX;
					projectileVelocity.y = projectileEmitter.velocity.y * directionY;

					Entity projectile = entity.registry->createEntity();
					projectile.addComponent<TransformComponent>(projectilePosition, glm::vec2(1.0, 1.0), 0);
					projectile.addComponent<RigidBodyComponent>(projectileVelocity);
					projectile.addComponent<SpriteComponent>("bullet-image", 4, 4, 4);
					projectile.addComponent<BoxColliderComponent>(4, 4);
					projectile.addComponent<ProjectileComponent>(projectileEmitter.isFriendly, projectileEmitter.hitPercentDamage, projectileEmitter.duration);
				}
			}
		}
	}

	void update(std::unique_ptr<Registry>& registry) {
		for (auto entity: getEntities()) {
			auto& projectileEmitter = entity.getComponent<ProjectileEmitterComponent>();
			const auto transform = entity.getComponent<TransformComponent>();

			if (projectileEmitter.repeatFrequency == 0) {
				continue;
			}

			if (SDL_GetTicks() - projectileEmitter.lastEmissionTime > projectileEmitter.repeatFrequency) {
				Entity projectile = registry->createEntity();

				glm::vec2 projectilePosition = transform.position;

				if (entity.hasComponent<SpriteComponent>()) {
					const auto sprite = entity.getComponent<SpriteComponent>();
					projectilePosition.x += transform.scale.x * sprite.width / 2;
					projectilePosition.y += transform.scale.y * sprite.height / 2;
				}

				projectile.addComponent<TransformComponent>(projectilePosition, glm::vec2(1.0, 1.0), 0);
				projectile.addComponent<RigidBodyComponent>(projectileEmitter.velocity);
				projectile.addComponent<SpriteComponent>("bullet-image", 4, 4, 4);
				projectile.addComponent<BoxColliderComponent>(4, 4);
				projectile.addComponent<ProjectileComponent>(projectileEmitter.isFriendly, projectileEmitter.hitPercentDamage, projectileEmitter.duration);
				projectileEmitter.lastEmissionTime = SDL_GetTicks();
			}
		}
	}

};

#endif