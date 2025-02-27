#pragma once
#include "Entity.h"
#include <vector>

namespace Engine {
namespace ECS {

// Base System class
class System {
private:
    ComponentMask componentMask;
    std::vector<Entity*> entities;
    bool active = true;

public:
    virtual ~System() = default;
    
    // Virtual functions for system lifecycle
    virtual void init() {}
    virtual void update(float deltaTime) {}
    virtual void render() {}
    
    bool isActive() const { return active; }
    void setActive(bool state) { active = state; }
    
    // Add entity to system
    void addEntity(Entity* entity) {
        entities.push_back(entity);
    }
    
    // Remove entity from system
    void removeEntity(Entity* entity) {
        entities.erase(
            std::remove(entities.begin(), entities.end(), entity),
            entities.end()
        );
    }
    
    // Get entities managed by this system
    const std::vector<Entity*>& getEntities() const {
        return entities;
    }
    
    // Set component mask for system
    template <typename... ComponentTypes>
    void setComponentMask() {
        componentMask.reset();
        (componentMask.set(getComponentID<ComponentTypes>()), ...);
    }
    
    // Check if entity matches system requirements
    bool isInterested(Entity* entity) const {
        return (entity->getComponentMask() & componentMask) == componentMask;
    }
    
    friend class ECSManager;
};

} // namespace ECS
} // namespace Engine
