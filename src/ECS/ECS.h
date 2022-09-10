#ifndef ECS_H
#define ECS_H

#include <bitset>
#include <vector>
#include <set>
#include <unordered_map>
#include <typeindex>
#include <memory>
#include <deque>
#include <iostream>

#include "../Logger/Logger.h"

// Constants
const unsigned int MAX_COMPONENTS = 32;

// We use a bitset (0s and 1s)
typedef std::bitset<MAX_COMPONENTS> Signature;

// Components
struct IComponent {

protected:
	static int nextId;

};

template <typename T> class Component : public IComponent {

public:
	static int getId() {
		static auto id = nextId++;
		return id;
	}

};

// Entities
class Entity {

private:
	int id;

public:
	Entity(int id): id(id) {};
	int getId() const;

	Entity& operator = (const Entity& other) = default;

	bool operator == (const Entity& other) const {
		return id == other.getId();
	}

	bool operator != (const Entity& other) const {
		return id != other.getId();
	}

	bool operator < (const Entity& other) const {
		return id < other.getId();
	}

	bool operator <= (const Entity& other) const {
		return id <= other.getId();
	}

	bool operator > (const Entity& other) const {
		return id > other.getId();
	}

	bool operator >= (const Entity& other) const {
		return id >= other.getId();
	}

	template <typename TComponent, typename ...TArgs> void addComponent(TArgs&& ...args);
	template <typename TComponent> void removeComponent();
	template <typename TComponent> bool hasComponent() const;
	template <typename TComponent> TComponent& getComponent() const;
	void kill();

	void tag(const std::string& tag);
	bool hasTag(const std::string& tag) const;
	void group(const std::string& group);
	bool belongsToGroup(const std::string& group) const;

	class Registry* registry;

};

// System
class System {

private:
	// Which components an entity must have for the system to consider the entity
	Signature componentSignature;

	// List of all entities that the system is interested in
	std::vector<Entity> entities;

public:
	System() = default;
	~System() = default;

	void addEntity(Entity entity);
	void removeEntity(Entity entity);
	std::vector<Entity> getEntities() const;
	const Signature& getComponentSignature() const;

	template <typename TComponent> void requireComponent();

};

// Registry
class IPool {
public:
	virtual ~IPool() {}

};

template <typename T> class Pool : public IPool {

private:
	std::vector<T> data;

public:
	Pool(int size = 100) {
		data.resize(size);
	}
	virtual ~Pool() = default;

	bool isEmpty() const {
		return data.empty();
	}

	int getSize() const {
		return data.size();
	}

	void resize(int size) {
		data.resize(size);
	}

	void clear() {
		data.clear();
	}

	void add(T object) {
		data.push_back(object);
	}

	void set(int index, T object) {
		data[index] = object;
	}

	T& get(int index) {
		return static_cast<T&>(data[index]);
	}

	T& operator [](unsigned int index) {
		return data[index];
	}

};

class Registry {

private:
	int numEntities = 0;
	std::set<Entity> entitiesToBeAdded;
	std::set<Entity> entitiesToBeKilled;

	std::vector<std::shared_ptr<IPool>> componentPools;

	std::vector<Signature> entityComponentSignatures;

	std::unordered_map<std::type_index, std::shared_ptr<System>> systems;

	std::deque<int> freeIds;

	std::unordered_map<std::string, Entity> entityPerTag;
	std::unordered_map<int, std::string> tagPerEntity;

	std::unordered_map<std::string, std::set<Entity>> entitiesPerGroup;
	std::unordered_map<int, std::string> groupPerEntity;

public:
	Registry() = default;
	~Registry() = default;

	// in this method we actually add or remove entities.
	// it's being executed at the end of update game loop method.
	void update();

	Entity createEntity();

	template <typename TComponent, typename ...TArgs> void addComponent(Entity entity, TArgs&& ...args);
	void killEntity(Entity entity);

	template <typename TComponent> void removeComponent(Entity entity);
	template <typename TComponent> bool hasComponent(Entity entity) const;
	template <typename TComponent> TComponent& getComponent(Entity entity) const;

	template <typename TSystem, typename ...TArgs> void addSystem(TArgs&& ...args);
	template <typename TSystem> void removeSystem();
	template <typename TSystem> bool hasSystem() const;
	template <typename TSystem> TSystem& getSystem() const;

	void addEntityToSystems(Entity entity);
	void removeEntityFromSystems(Entity entity);

	void tagEntity(Entity entity, const std::string& tag);
	bool entityHasTag(Entity entity, const std::string& tag) const;
	Entity getEntityByTag(const std::string& tag) const;
	void removeEntityTag(Entity entity);

	void groupEntity(Entity entity, const std::string& group);
	bool entityBelongsToGroup(Entity entity, const std::string& group) const;
	std::vector<Entity> getEntitiesByGroup(const std::string& group) const;
	void removeEntityGroup(Entity entity);

};


// TEMPLATES

// Registry templates
template <typename TComponent, typename ...TArgs> void Registry::addComponent(Entity entity, TArgs&& ...args) {
	const auto componentId = Component<TComponent>::getId();
	const auto entityId = entity.getId();

	if (componentId >= componentPools.size()) {
		componentPools.resize(componentId + 1, nullptr);
	}

	if (!componentPools[componentId]) {
		std::shared_ptr<Pool<TComponent>> newComponentPool = std::make_shared<Pool<TComponent>>();
		componentPools[componentId] = newComponentPool;
	}

	std::shared_ptr<Pool<TComponent>> componentPool = std::static_pointer_cast<Pool<TComponent>>(componentPools[componentId]);

	if (entityId >= componentPool->getSize()) {
		componentPool->resize(numEntities);
	}

	TComponent newComponent(std::forward<TArgs>(args)...);

	componentPool->set(entityId, newComponent);

	entityComponentSignatures[entityId].set(componentId);

	//Logger::info("Component id " + std::to_string(componentId) + " added to the entity id = " + std::to_string(entityId));
}

template <typename TComponent> void Registry::removeComponent(Entity entity) {
	const auto componentId = Component<TComponent>::getId();
	const auto entityId = entity.getId();

	entityComponentSignatures[entityId].set(componentId, false);
}


template <typename TComponent> bool Registry::hasComponent(Entity entity) const {
	const auto componentId = Component<TComponent>::getId();
	const auto entityId = entity.getId();

	return entityComponentSignatures[entityId].test(componentId);
}

template <typename TComponent> TComponent& Registry::getComponent(Entity entity) const {
	const auto componentId = Component<TComponent>::getId();
	const auto entityId = entity.getId();
	auto componentPool = std::static_pointer_cast<Pool<TComponent>>(componentPools[componentId]);
	return componentPool->get(entityId);
}

template <typename TSystem, typename ...TArgs> void Registry::addSystem(TArgs&& ...args) {
	const std::shared_ptr<TSystem> newSystem = std::make_shared<TSystem>(std::forward<TArgs>(args)...);
	systems.insert(std::make_pair(std::type_index(typeid(TSystem)),newSystem));
}

template <typename TSystem> void Registry::removeSystem() {
	const auto index = systems.find(std::type_index(typeid(TSystem)));
	systems.erase(index);
}

template <typename TSystem> bool Registry::hasSystem() const {
	const auto index = std::type_index(typeid(TSystem));
	return systems.find(index) != systems.end();
}

template <typename TSystem> TSystem& Registry::getSystem() const {
	const auto system = systems.find(std::type_index(typeid(TSystem)));
	return *(std::static_pointer_cast<TSystem>(system->second));
}

// Entity templates
template <typename TComponent, typename ...TArgs> void Entity::addComponent(TArgs&& ...args) {
	registry->addComponent<TComponent>(*this, std::forward<TArgs>(args)...);	
}

template <typename TComponent> void Entity::removeComponent() {
	registry->removeComponent<TComponent>(*this);
}

template <typename TComponent> bool Entity::hasComponent() const {
	return registry->hasComponent<TComponent>(*this);	
}

template <typename TComponent> TComponent& Entity::getComponent() const {
	return registry->getComponent<TComponent>(*this);
}

// System templates
template <typename TComponent> void System::requireComponent() {
	const auto componentId = Component<TComponent>::getId();
	componentSignature.set(componentId);
}


#endif