#pragma once
#include "Entity.h"
#include "Component.h"
#include "System.h"
#include <memory>
#include <vector>
#include <array>
#include <unordered_map>
#include <string>
#include <chrono>

// Forward declarations for external classes
namespace engine {
    namespace rendering {
        class Window;
        class Texture;
        class Model;
        class Shader;
        class Material;
    }
    namespace core {
        class ResourceManager;
    }
}

namespace Engine {
namespace ECS {

class System;
class Entity;
class Component;
class RenderSystem;

class ECSManager {
    friend class Entity;

private:
    // Entity management
    std::array<std::unique_ptr<Entity>, MAX_ENTITIES> entities;
    std::vector<EntityID> freeEntities;
    std::size_t livingEntityCount = 0;
    
    // Component storage - vector of component arrays
    std::array<std::vector<std::unique_ptr<Component>>, MAX_COMPONENTS> components;
    
    // System management
    std::vector<std::unique_ptr<System>> systems;

    static ::engine::rendering::Window* window;

public:
    ECSManager();

    static engine::rendering::Window* getWindow() {
        return window;
    }
    
    // Create a new entity
    Entity* createEntity();
    
    // Destroy an entity
    void destroyEntity(EntityID id);
    
    // Get entity by ID
    Entity* getEntity(EntityID id);
    
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
        // Important: Set the manager reference right after creation
        system->setManagerInternal(this);
        system->template setComponentMask<ComponentTypes...>();
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
    void update(float deltaTime);
    
    // Render all systems
    void render();
    
    // Clean up destroyed entities
    void refresh();
    
    void initialize(engine::rendering::Window* gameWindow);
    void runGameLoop();
    void processInput(float deltaTime);
    void shutdown();
    
    // Resource loading helpers
    static std::shared_ptr<engine::rendering::Texture> loadTexture(const std::string& path);
    static std::shared_ptr<engine::rendering::Model> loadModel(const std::string& path);
    static std::shared_ptr<engine::rendering::Shader> loadShader(
        const std::string& name,
        const std::string& vertexPath,
        const std::string& fragmentPath);
    static std::shared_ptr<engine::rendering::Material> createMaterial(const std::string& name);

    std::vector<Entity*> getAllEntities() const;
};

// Implementation of Entity template methods - now that ECSManager is fully defined
template <typename T, typename... Args>
T& Entity::addComponent(Args&&... args) {
    return manager->addComponent<T>(this, std::forward<Args>(args)...);
}

template <typename T>
T& Entity::getComponent() {
    static_assert(std::is_base_of<Component, T>::value, "T must derive from Component");
    
    ComponentID id = getComponentID<T>();
    
    if (!hasComponent<T>()) {
        throw std::runtime_error("Entity does not have requested component");
    }
    
    // We need to access the component through the manager
    auto& componentArray = manager->components[id];
    if (componentArray.size() <= this->id || !componentArray[this->id]) {
        throw std::runtime_error("Component exists in mask but not in storage");
    }
    
    return *static_cast<T*>(componentArray[this->id].get());
}

template <typename T>
void Entity::removeComponent() {
    static_assert(std::is_base_of<Component, T>::value, "T must derive from Component");
    
    ComponentID id = getComponentID<T>();
    
    if (!hasComponent<T>()) {
        return; // No component to remove
    }
    
    // Remove component from storage
    if (manager->components[id].size() > this->id) {
        manager->components[id][this->id].reset();
    }
    
    // Update component mask
    componentMask.reset(id);
    
    // Update systems (remove entity from systems that no longer match)
    for (auto& system : manager->systems) {
        if (!system->isInterested(this) && 
            std::find(system->getEntities().begin(), system->getEntities().end(), this) != system->getEntities().end()) {
            system->removeEntity(this);
        }
    }
}

} // namespace ECS
} // namespace Engine
