#ifndef DAMAGESYSTEM_H
#define DAMAGESYSTEM_H

#include "../Logger/Logger.h"
#include "../ECS/ECS.h"
#include "../EventBus/EventBus.h"
#include "../Components/BoxColliderComponent.h"
#include "../Components/ProjectileComponent.h"
#include "../Components/HealthComponent.h"
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
		Entity a = event.a;
		Entity b = event.b;

		if (a.belongsToGroup("projectiles") && b.hasTag("player")) {
			onProjectileHitsPlayer(a, b);
		}

		if (b.belongsToGroup("projectiles") && a.hasTag("player")) {
			onProjectileHitsPlayer(b, a);
		}

		if (a.belongsToGroup("projectiles") && b.belongsToGroup("enemies")) {
			onProjectileHitsEnemy(a, b);
		}

		if (b.belongsToGroup("projectiles") && a.belongsToGroup("enemies")) {
			onProjectileHitsEnemy(b, a);
		}
	}

	void onProjectileHitsPlayer(Entity projectile, Entity player) {
		Logger::info("Player took a damage due to a collision with a projectile");

		auto projectileComponent = projectile.getComponent<ProjectileComponent>();

		if (!projectileComponent.isFriendly) {
			auto& health = player.getComponent<HealthComponent>();

			health.healthPercentage -= projectileComponent.hitPercentDamage;
			Logger::info("Player has been hit in " + std::to_string(projectileComponent.hitPercentDamage) 
				+ " out of " + std::to_string(health.healthPercentage) + " health percentage");

			if (health.healthPercentage <= 0) {
				player.kill();
			}

			projectile.kill();
		}
	}

	void onProjectileHitsEnemy(Entity projectile, Entity enemy) {
		Logger::info("Enemy took a damage due to a collision with a projectile");

		auto projectileComponent = projectile.getComponent<ProjectileComponent>();

		if (projectileComponent.isFriendly) {
			auto& health = enemy.getComponent<HealthComponent>();

			health.healthPercentage -= projectileComponent.hitPercentDamage;
			Logger::info("Enemy has been hit in " + std::to_string(projectileComponent.hitPercentDamage) 
				+ " out of " + std::to_string(health.healthPercentage) + " health percentage");

			if (health.healthPercentage <= 0) {
				enemy.kill();
			}

			projectile.kill();
		}
	}

	void update() {

	}

};

#endif