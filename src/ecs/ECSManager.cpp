#include "include/ecs/ECSManager.h"

namespace Engine {
namespace ECS {

ECSManager::ECSManager() {
    // Initialize free entity IDs
    for (EntityID i = 0; i < MAX_ENTITIES; i++) {
        freeEntities.push_back(i);
    }
}

Entity* ECSManager::createEntity() {
    if (freeEntities.empty()) {
        throw std::runtime_error("Maximum number of entities reached");
    }
    
    EntityID id = freeEntities.back();
    freeEntities.pop_back();
    livingEntityCount++;
    
    entities[id] = std::make_unique<Entity>(id, this);
    return entities[id].get();
}

void ECSManager::destroyEntity(EntityID id) {
    if (id >= MAX_ENTITIES || !entities[id]) {
        return;
    }
    
    entities[id]->destroy();
}

Entity* ECSManager::getEntity(EntityID id) {
    if (id >= MAX_ENTITIES) {
        return nullptr;
    }
    return entities[id].get();
}

void ECSManager::update(float deltaTime) {
    for (auto& system : systems) {
        if (system->isActive()) {
            system->update(deltaTime);
        }
    }
}

void ECSManager::render() {
    for (auto& system : systems) {
        if (system->isActive()) {
            system->render();
        }
    }
}

void ECSManager::refresh() {
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

} // namespace ECS
} // namespace Engine
