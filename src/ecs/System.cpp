#include "ecs/System.h"
#include <algorithm>
#include <iostream>

namespace Engine {
namespace ECS {

// Implement virtual destructor
System::~System() = default;

// Implement addEntity method
void System::addEntity(Entity* entity) {
    std::cout << "System: Adding entity " << entity->getID() << std::endl;
    // Only add if not already in the list
    if (std::find(entities.begin(), entities.end(), entity) == entities.end()) {
        entities.push_back(entity);
    }
}

// Implement removeEntity method
void System::removeEntity(Entity* entity) {
    std::cout << "System: Removing entity " << entity->getID() << std::endl;
    entities.erase(
        std::remove(entities.begin(), entities.end(), entity),
        entities.end()
    );
}

} // namespace ECS
} // namespace Engine
