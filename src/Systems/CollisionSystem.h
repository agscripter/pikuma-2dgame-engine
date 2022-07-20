#ifndef COLLISIONSYSTEM_H
#define COLLISIONSYSTEM_H

#include "../Logger/Logger.h"
#include "../ECS/ECS.h"
#include "../EventBus/EventBus.h"
#include "../Components/TransformComponent.h"
#include "../Components/BoxColliderComponent.h"
#include "../Events/CollisionEvent.h"

class CollisionSystem : public System {

public:
	CollisionSystem() {
		requireComponent<TransformComponent>();
		requireComponent<BoxColliderComponent>();
	}

	void update(std::unique_ptr<EventBus>& eventBus) {
		auto entities = getEntities();

		for (auto i = entities.begin(); i != entities.end(); i++) {
			Entity entity = *i;

			auto entityTransform = entity.getComponent<TransformComponent>();
			auto entityBoxCollider = entity.getComponent<BoxColliderComponent>();

			for (auto j = i; j < entities.end(); j++) {
				Entity otherEntity = *j;

				if (entity == otherEntity) {
					continue;
				}

				auto otherEntityTransform = otherEntity.getComponent<TransformComponent>();
				auto otherEntityBoxCollider = otherEntity.getComponent<BoxColliderComponent>();

				if (checkCollision(entityTransform, entityBoxCollider, 
					otherEntityTransform, otherEntityBoxCollider)) {
					Logger::info("Entity id = " + std::to_string(entity.getId()) + " collided with " + std::to_string(otherEntity.getId()));
					eventBus->emit<CollisionEvent>(entity, otherEntity);
				}
			}
		}
	}

	bool checkCollision(TransformComponent aTransform, BoxColliderComponent aCollider,
		TransformComponent bTransform, BoxColliderComponent bCollider) {

		return (
			(aTransform.position.x + aCollider.offset.x) < (bTransform.position.x + bCollider.offset.x) + bCollider.width  &&
			(aTransform.position.x + aCollider.offset.x) + aCollider.width > (bTransform.position.x + bCollider.offset.x)  &&
			(aTransform.position.y + aCollider.offset.y) < (bTransform.position.y + bCollider.offset.y) + bCollider.height &&
			(aTransform.position.y + aCollider.offset.y) + aCollider.height > (bTransform.position.y + bCollider.offset.y)
		);
	}

};

#endif