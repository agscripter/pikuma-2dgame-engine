#ifndef MOVEMENTSYSTEM_H
#define MOVEMENTSYSTEM_H

#include "../ECS/ECS.h"
#include "../EventBus/EventBus.h"
#include "../Events/CollisionEvent.h"
#include "../Components/TransformComponent.h"
#include "../Components/RigidBodyComponent.h"
#include "../Components/SpriteComponent.h"
#include "../Logger/Logger.h"

class MovementSystem : public System {

public:
	MovementSystem() {
		requireComponent<TransformComponent>();
		requireComponent<RigidBodyComponent>();
	}

	void update(double deltaTime) {
		for (auto entity: getEntities()) {
			auto& transform = entity.getComponent<TransformComponent>();
            const auto rigidbody = entity.getComponent<RigidBodyComponent>();

            transform.position.x += rigidbody.velocity.x * deltaTime;
            transform.position.y += rigidbody.velocity.y * deltaTime; 

            if (entity.hasTag("player")) {
            	int paddingLeft = 10;
            	int paddingRight = 50;
            	int paddingTop = 10;
            	int paddingBottom = 50;
            	transform.position.x = transform.position.x < paddingLeft ? paddingLeft : transform.position.x;
            	transform.position.x = transform.position.x > Game::mapWidth - paddingRight ? Game::mapWidth - paddingRight : transform.position.x;

				transform.position.y = transform.position.y < paddingTop ? paddingTop : transform.position.y;
            	transform.position.y = transform.position.y > Game::mapHeight - paddingBottom ? Game::mapHeight - paddingBottom : transform.position.y;
            }

            bool isEntityOutsideMap = (
            	transform.position.x < 0 || transform.position.x > Game::mapWidth ||
            	transform.position.x < 0 || transform.position.x > Game::mapHeight
            );

            if (isEntityOutsideMap && !entity.hasTag("player")) {
            	entity.kill();
            }
		}
	}

	void subscribeToEvents(const std::unique_ptr<EventBus>& eventBus) {
		eventBus->subscribeToEvent<CollisionEvent>(this, &MovementSystem::onCollision);
	}

	void onCollision(CollisionEvent& event) {
		Entity a = event.a;
		Entity b = event.b;

		if (a.belongsToGroup("enemies") && b.belongsToGroup("obstacles")) {
			onEnemyHitsObstacles(a, b);
		}

		if (b.belongsToGroup("enemies") && a.belongsToGroup("obstacles")) {
			onEnemyHitsObstacles(b, a);
		}
	}

	void onEnemyHitsObstacles(Entity enemy, Entity obstacle){
		if (enemy.hasComponent<RigidBodyComponent>()) {
			auto& rigidbody = enemy.getComponent<RigidBodyComponent>();

			if (rigidbody.velocity.x != 0) {
				rigidbody.velocity.x *= -1;
			}

			if (rigidbody.velocity.y != 0) {
				rigidbody.velocity.y *= -1;
			}
		}

		if (enemy.hasComponent<SpriteComponent>()) {
			auto& sprite = enemy.getComponent<SpriteComponent>();
			sprite.flip = (sprite.flip == SDL_FLIP_NONE) ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE;
		}
	}

};

#endif