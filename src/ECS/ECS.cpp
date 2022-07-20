#include "ECS.h"
#include "../Logger/Logger.h"

// Component
int IComponent::nextId = 0;

// Entity
int Entity::getId() const {
	return id;
}

void Entity::kill() {
	registry->killEntity(*this);
}

void Entity::tag(const std::string& tag) {
	registry->tagEntity(*this, tag);
}

bool Entity::hasTag(const std::string& tag) const {
	return registry->entityHasTag(*this, tag);
}

void Entity::group(const std::string& group) {
	registry->groupEntity(*this, group);
}

bool Entity::belongsToGroup(const std::string& group) const {
	return registry->entityBelongsToGroup(*this, group);
}

// System
void System::addEntity(Entity entity) {
	entities.push_back(entity);
};

void System::removeEntity(Entity entity) {
	entities.erase(std::remove_if(entities.begin(), entities.end(), [&entity](Entity other) {
		return entity == other;
	}), entities.end());
};

std::vector<Entity> System::getEntities() const {
	return entities;
};

const Signature& System::getComponentSignature() const {
	return componentSignature;
};


// Registry
Entity Registry::createEntity() {
	int entityId;

	if (freeIds.empty()) {
		entityId = numEntities++;

		if (entityId >= entityComponentSignatures.size()) {
			entityComponentSignatures.resize(entityId + 1);
		}
	} else {
		entityId = freeIds.front();
		freeIds.pop_front();
	}

	Logger::info("Entity created with id = " + std::to_string(entityId));

	Entity entity(entityId);
	entity.registry = this;
	entitiesToBeAdded.insert(entity);
	return entity;
};

void Registry::killEntity(Entity entity) {
	entitiesToBeKilled.insert(entity);
    Logger::info("Entity " + std::to_string(entity.getId()) + " was killed");
}

void Registry::update() {
	for (auto entity: entitiesToBeAdded) {
		addEntityToSystems(entity);
	}

	entitiesToBeAdded.clear();

	for (auto entity: entitiesToBeKilled) {
		removeEntityFromSystems(entity);

		entityComponentSignatures[entity.getId()].reset();

		freeIds.push_back(entity.getId());
	}
	entitiesToBeKilled.clear();
};

void Registry::addEntityToSystems(Entity entity) {
	const auto entityId = entity.getId();

	const auto& entityComponentSignature = entityComponentSignatures[entityId];

	for (auto& system: systems) {
		const auto& systemComponentSignature = system.second->getComponentSignature();

		bool isInterested = (entityComponentSignature & systemComponentSignature) == systemComponentSignature;

		if (isInterested) {
			system.second->addEntity(entity);
		}
	}

}

void Registry::removeEntityFromSystems(Entity entity) {
	for (auto system : systems) {
		system.second->removeEntity(entity);
	}
}

void Registry::tagEntity(Entity entity, const std::string& tag) {
	entityPerTag.emplace(tag, entity);
	tagPerEntity.emplace(entity.getId(), tag);
}

bool Registry::entityHasTag(Entity entity, const std::string& tag) const {
	if (tagPerEntity.find(entity.getId()) == tagPerEntity.end()) {
		return false;
	}
	return entityPerTag.at(tag);
}

Entity Registry::getEntityByTag(const std::string& tag) const {
	return entityPerTag.at(tag);
}

void Registry::removeEntityTag(Entity entity) {
	auto taggedEntity = tagPerEntity.find(entity.getId());
	if (taggedEntity != tagPerEntity.end()) {
		auto tag = taggedEntity->second;
		entityPerTag.erase(tag);
		tagPerEntity.erase(taggedEntity);
	}
}

void Registry::groupEntity(Entity entity, const std::String& group) {
	entitiesPerGroup.emplace(group, std::set<Entity>());
	entitiesPerGroup[group].emplace(entity);
	groupPerEntity.emplace(entity.getId(), group);
}

bool Registry::entityBelongsToGroup(Entity entity, const std::string& group) const {
	
}

