#ifndef MOVEMENTSYSTEM_H
#define MOVEMENTSYSTEM_H

#include "../ECS/ECS.h"
#include "../Components/TransformComponent.h"
#include "../Components/RigidBodyComponent.h"
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
		}
	}

};

#endif