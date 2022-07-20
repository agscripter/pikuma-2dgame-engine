#ifndef RIGIDBODYCOMPONENT_H
#define RIGIDBODYCOMPONENT_H

#include <glm/glm.hpp>
#include "../ECS/ECS.h"

struct RigidBodyComponent : IComponent {
	glm::vec2 velocity;

	RigidBodyComponent(glm::vec2 velocity = glm::vec2(0, 0)) {
		this->velocity = velocity;
	}
};

#endif 