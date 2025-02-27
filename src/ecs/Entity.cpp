#include "include/ecs/Entity.h"
#include "include/ecs/components/TransformComponent.h"
#include "include/ecs/Component.h"
#include "include/ecs/ECSManager.h"

namespace Engine {
namespace ECS {


void Entity::setParent(Entity* newParent) {
    // Remove from old parent
    if (parent) {
        auto& oldChildren = parent->children;
        oldChildren.erase(std::remove(oldChildren.begin(), oldChildren.end(), this), oldChildren.end());
    }
    
    // Set new parent
    parent = newParent;
    
    // Add to new parent's children
    if (parent) {
        parent->children.push_back(this);
    }
    
    // Mark transforms as dirty
    if (hasComponent<TransformComponent>()) {
        getComponent<TransformComponent>().worldMatrixDirty = true;
    }
}

Entity* Entity::getParent() const {
    return parent;
}

const std::vector<Entity*>& Entity::getChildren() const {
    return children;
}

// Most Entity methods are templated and defined in the header,
// but we can implement any non-templated methods here

// If we decide to add any non-templated methods that aren't inline,
// they would go here

} // namespace ECS
} // namespace Engine
