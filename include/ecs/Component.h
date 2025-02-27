#pragma once
#include "Entity.h"

namespace Engine {
namespace ECS {

// Base Component class
class Component {
private:
    Entity* owner = nullptr;
    bool active = true;

public:
    virtual ~Component() = default;
    
    // Virtual functions for component lifecycle
    virtual void init() {}
    virtual void update(float deltaTime) {}
    virtual void render() {}
    
    bool isActive() const { return active; }
    void setActive(bool state) { active = state; }
    
    Entity* getOwner() const { return owner; }
    void setOwner(Entity* entity) { owner = entity; }
};

} // namespace ECS
} // namespace Engine
