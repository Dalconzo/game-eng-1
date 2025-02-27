#include "include/ecs/System.h"
#include <algorithm>

namespace Engine {
namespace ECS {

// Implement virtual destructor
System::~System() = default;

// Implement addEntity method
void System::addEntity(Entity* entity) {
    // Only add if not already in the list
    if (std::find(entities.begin(), entities.end(), entity) == entities.end()) {
        entities.push_back(entity);
    }
}

// Implement removeEntity method
void System::removeEntity(Entity* entity) {
    entities.erase(
        std::remove(entities.begin(), entities.end(), entity),
        entities.end()
    );
}

} // namespace ECS
} // namespace Engine
