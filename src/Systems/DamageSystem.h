#ifndef DAMAGESYSTEM_H
#define DAMAGESYSTEM_H

#include "../Logger/Logger.h"
#include "../ECS/ECS.h"
#include "../EventBus/EventBus.h"
#include "../Components/BoxColliderComponent.h"
#include "../Events/CollisionEvent.h"

class DamageSystem : public System {

public:
	DamageSystem() {
		requireComponent<BoxColliderComponent>();
	}

	void subscribeToEvents(std::unique_ptr<EventBus>& eventBus) {
		eventBus->subscribeToEvent<CollisionEvent>(this, &DamageSystem::onCollision);
	}

	void onCollision(CollisionEvent& event) {
		Logger::info("Entity took a damage due to a collision");
		// event.a.kill();
		// event.b.kill();
	}

	void update() {

	}

};

#endif