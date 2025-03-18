#ifndef ECS_MANAGER_H
#define ECS_MANAGER_H

#include <unordered_map>
#include <memory>
#include <typeindex>
#include <vector>
#include "System.h"
#include "ComponentPool.h"
#include "Entity.h"

using Entity = uint32_t;

class ECSManager {
public:
    Entity nextEntityId = 0;
    ECSManager() : nextEntityId(0), playerEntity(0) {}

    Entity createEntity() {
        Entity entity = nextEntityId++;
        entityComponents[entity] = std::vector<size_t>();
        return entity;
    }

    void destroyEntity(Entity entity) {
        for (auto& poolPair : componentPools) {
            poolPair.second->remove(entity); // Remove entity from all pools
        }
        entityComponents.erase(entity); // Remove entity’s component list
    }

    template <typename T>
    T& addComponent(Entity entity) {
        auto pool = getOrCreatePool<T>();
        T& component = pool->add(entity);
        size_t typeId = getComponentTypeId(typeid(T));
        entityComponents[entity].push_back(typeId);
        return component;
    }

    template <typename T>
    T* getComponent(Entity entity) {
        auto it = componentPools.find(typeid(T));
        if (it != componentPools.end()) {
            ComponentPool<T>* pool = static_cast<ComponentPool<T>*>(it->second.get());
            return pool->get(entity);
        }
        return nullptr;
    }

    template <typename T>
    void removeComponent(Entity entity) {
        auto it = componentPools.find(typeid(T));
        if (it != componentPools.end()) {
            ComponentPool<T>* pool = static_cast<ComponentPool<T>*>(it->second.get());
            pool->remove(entity); // Remove from pool
            size_t typeId = getComponentTypeId(typeid(T));
            auto& comps = entityComponents[entity];
            comps.erase(std::remove(comps.begin(), comps.end(), typeId), comps.end()); // Remove type ID
        }
    }

    void registerSystem(std::unique_ptr<System> system) {
        systems.push_back(std::move(system));
    }

    void update() {
        for (auto& system : systems) {
            system->update(this);
        }
    }

    // Add player entity getter and setter
    Entity getPlayerEntity() const {
        return playerEntity;
    }

    void setPlayerEntity(Entity entity) {
        playerEntity = entity;
    }

    static size_t getComponentTypeId(const std::type_info& ti);
    std::vector<Entity> getEntitiesWithComponents(const std::vector<size_t>& componentTypes);

private:
    std::unordered_map<std::type_index, std::unique_ptr<ComponentPoolBase>> componentPools;
    std::vector<std::unique_ptr<System>> systems;
    std::unordered_map<Entity, std::vector<size_t>> entityComponents;
    Entity playerEntity; // Store the player entity ID

    template <typename T>
    ComponentPool<T>* getOrCreatePool() {
        std::type_index type(typeid(T));
        auto it = componentPools.find(type);
        if (it == componentPools.end()) {
            auto pool = std::make_unique<ComponentPool<T>>();
            ComponentPool<T>* poolPtr = pool.get();
            componentPools[type] = std::move(pool);
            return poolPtr;
        }
        return static_cast<ComponentPool<T>*>(it->second.get());
    }
};

#endif // ECS_MANAGER_H