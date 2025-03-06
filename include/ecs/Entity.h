#pragma once
#include <cstdint>
#include <bitset>
#include <array>
#include <unordered_map>
#include <vector>
#include <algorithm>
#include <stdexcept>
#include <memory>

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

// Template function to get component ID for a specific type
template <typename T>
inline ComponentID getComponentID() {
    static_assert(std::is_base_of<Component, T>::value, "T must derive from Component");
    static ComponentID typeID = getNewComponentID();
    return typeID;
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

    ECSManager* getManager() const { return manager; }
    
    EntityID getID() const { return id; }
    
    bool isActive() const { return active; }
    void destroy() { active = false; }
    
    ComponentMask getComponentMask() const { return componentMask; }
    
    // Check if entity has a specific component
    template <typename T>
    bool hasComponent() const {
        return componentMask[getComponentID<T>()];
    }
    
    // Add component to entity - declaration only
    template <typename T, typename... Args>
    T& addComponent(Args&&... args);
    
    // Get component from entity - declaration only
    template <typename T>
    T& getComponent();
    
    // Remove component from entity - declaration only
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

} // namespace ECS
} // namespace Engine

// Note: Template implementations moved to ECSManager.h to break circular dependency
