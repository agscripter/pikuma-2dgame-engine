#ifndef KEYBOARDCONTROLSYSTEM_H
#define KEYBOARDCONTROLSYSTEM_H

#include "../Logger/Logger.h"
#include "../ECS/ECS.h"
#include "../EventBus/EventBus.h"
#include "../Events/KeyPressedEvent.h"
#include "../Components/RigidBodyComponent.h"
#include "../Components/SpriteComponent.h"
#include "../Components/KeyboardControlledComponent.h"

class KeyboardControlSystem : public System {

public:
	KeyboardControlSystem() {
		requireComponent<SpriteComponent>();
		requireComponent<KeyboardControlledComponent>();
		requireComponent<RigidBodyComponent>();
	}	

	void subscribeToEvents(std::unique_ptr<EventBus>& eventBus) {
		eventBus->subscribeToEvent<KeyPressedEvent>(this, &KeyboardControlSystem::onKeyPressed);
	}

	void onKeyPressed(KeyPressedEvent& event) {
		for (auto entity: getEntities()) {
			const auto keyboardControl = entity.getComponent<KeyboardControlledComponent>();
			auto& rigidbody = entity.getComponent<RigidBodyComponent>();
			auto& sprite = entity.getComponent<SpriteComponent>();

			switch(event.symbol) {
				case SDLK_UP:
					rigidbody.velocity = keyboardControl.upVelocity;
					sprite.sourceRect.y = sprite.height * 0;
					break;
				case SDLK_RIGHT:
					rigidbody.velocity = keyboardControl.rightVelocity;
					sprite.sourceRect.y = sprite.height * 1;
					break;
				case SDLK_DOWN:
					rigidbody.velocity = keyboardControl.downVelocity;
					sprite.sourceRect.y = sprite.height * 2;
					break;
				case SDLK_LEFT:
					rigidbody.velocity = keyboardControl.leftVelocity;
					sprite.sourceRect.y = sprite.height * 3;
					break;
			}
		}
	}

	void update() {
		
	}

};

#endif