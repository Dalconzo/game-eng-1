#pragma once
#include "Entity.h"
#include "Component.h"
#include "System.h"
#include <memory>
#include <vector>
#include <array>
#include <unordered_map>

namespace Engine {
namespace ECS {

class ECSManager {
private:
    // Entity management
    std::array<std::unique_ptr<Entity>, MAX_ENTITIES> entities;
    std::vector<EntityID> freeEntities;
    std::size_t livingEntityCount = 0;
    
    // Component storage - vector of component arrays
    std::array<std::vector<std::unique_ptr<Component>>, MAX_COMPONENTS> components;
    
    // System management
    std::vector<std::unique_ptr<System>> systems;

public:
    ECSManager() {
        // Initialize free entity IDs
        for (EntityID i = 0; i < MAX_ENTITIES; i++) {
            freeEntities.push_back(i);
        }
    }
    
    // Create a new entity
    Entity* createEntity() {
        if (freeEntities.empty()) {
            throw std::runtime_error("Maximum number of entities reached");
        }
        
        EntityID id = freeEntities.back();
        freeEntities.pop_back();
        livingEntityCount++;
        
        entities[id] = std::make_unique<Entity>(id, this);
        return entities[id].get();
    }
    
    // Destroy an entity
    void destroyEntity(EntityID id) {
        if (id >= MAX_ENTITIES || !entities[id]) {
            return;
        }
        
        entities[id]->destroy();
    }
    
    // Get entity by ID
    Entity* getEntity(EntityID id) {
        if (id >= MAX_ENTITIES) {
            return nullptr;
        }
        return entities[id].get();
    }
    
    // Component management methods
    template <typename T, typename... Args>
    T& addComponent(Entity* entity, Args&&... args) {
        ComponentID id = getComponentID<T>();
        
        // Ensure component storage exists
        if (components[id].size() <= entity->getID()) {
            components[id].resize(entity->getID() + 1);
        }
        
        // Create component
        components[id][entity->getID()] = std::make_unique<T>(std::forward<Args>(args)...);
        auto component = static_cast<T*>(components[id][entity->getID()].get());
        component->setOwner(entity);
        component->init();
        
        // Update entity's component mask
        entity->componentMask.set(id);
        
        // Add entity to interested systems
        for (auto& system : systems) {
            if (system->isInterested(entity)) {
                system->addEntity(entity);
            }
        }
        
        return *component;
    }
    
    // Register a system
    template <typename T, typename... ComponentTypes>
    T* registerSystem() {
        // Create system
        auto system = std::make_unique<T>();
        system->setComponentMask<ComponentTypes...>();
        system->init();
        
        // Find entities that match system requirements
        for (std::size_t i = 0; i < MAX_ENTITIES; i++) {
            if (entities[i] && entities[i]->isActive() && system->isInterested(entities[i].get())) {
                system->addEntity(entities[i].get());
            }
        }
        
        T* systemPtr = system.get();
        systems.push_back(std::move(system));
        return systemPtr;
    }
    
    // Update all systems
    void update(float deltaTime) {
        for (auto& system : systems) {
            if (system->isActive()) {
                system->update(deltaTime);
            }
        }
    }
    
    // Render all systems
    void render() {
        for (auto& system : systems) {
            if (system->isActive()) {
                system->render();
            }
        }
    }
    
    // Clean up destroyed entities
    void refresh() {
        for (std::size_t i = 0; i < MAX_ENTITIES; i++) {
            if (entities[i] && !entities[i]->isActive()) {
                // Remove entity from all systems
                for (auto& system : systems) {
                    system->removeEntity(entities[i].get());
                }
                
                // Reset components
                for (std::size_t j = 0; j < MAX_COMPONENTS; j++) {
                    if (components[j].size() > i && components[j][i]) {
                        components[j][i].reset();
                    }
                }
                
                // Free entity ID
                entities[i].reset();
                freeEntities.push_back(static_cast<EntityID>(i));
                livingEntityCount--;
            }
        }
    }
};

// Implement addComponent function from Entity class
template <typename T, typename... Args>
T& Entity::addComponent(Args&&... args) {
    return manager->addComponent<T>(this, std::forward<Args>(args)...);
}

} // namespace ECS
} // namespace Engine
