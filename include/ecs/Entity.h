#pragma once
#include <cstdint>
#include <bitset>
#include <array>
#include <unordered_map>

namespace Engine {
namespace ECS {

// Forward declarations
class Component;
class System;
class ECSManager;
class TransformComponent;

// Maximum number of components and entities
constexpr std::size_t MAX_COMPONENTS = 32;
constexpr std::size_t MAX_ENTITIES = 10000;

// Component ID and ComponentMask types
using ComponentID = std::size_t;
using ComponentMask = std::bitset<MAX_COMPONENTS>;

// Entity ID type
using EntityID = std::uint32_t;

// Component ID generator
inline ComponentID getNewComponentID() {
    static ComponentID lastID = 0;
    return lastID++;
}

// Entity class
class Entity {
private:
    EntityID id;
    ComponentMask componentMask;
    ECSManager* manager;
    bool active = true;
    Entity* parent = nullptr;
    std::vector<Entity*> children;

public:
    Entity(EntityID id, ECSManager* manager) : id(id), manager(manager) {}
    
    EntityID getID() const { return id; }
    
    bool isActive() const { return active; }
    void destroy() { active = false; }
    
    ComponentMask getComponentMask() const { return componentMask; }
    
    // Check if entity has a specific component
    template <typename T>
    bool hasComponent() const {
        return componentMask[getComponentID<T>()];
    }
    
    // Add component to entity
    template <typename T, typename... Args>
    T& addComponent(Args&&... args);
    
    // Get component from entity
    template <typename T>
    T& getComponent();
    
    // Remove component from entity
    template <typename T>
    void removeComponent();
    
    friend class ECSManager;

    Entity* getParent() const { return parent; }
    
    void setParent(Entity* newParent);
    
    void addChild(Entity* child) {
        // Ensure the child isn't already in our children list
        if (std::find(children.begin(), children.end(), child) == children.end()) {
            children.push_back(child);
        }
    }
    
    void removeChild(Entity* child) {
        children.erase(
            std::remove(children.begin(), children.end(), child),
            children.end()
        );
    }
    
    const std::vector<Entity*>& getChildren() const {
        return children;
    }
};

// Template function to get component ID for a specific type
template <typename T>
inline ComponentID getComponentID() {
    static_assert(std::is_base_of<Component, T>::value, "T must derive from Component");
    static ComponentID typeID = getNewComponentID();
    return typeID;
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
