#pragma once
#include "../System.h"
#include "../components/CameraControllerComponent.h"
#include "../components/TransformComponent.h"

namespace Engine {
namespace ECS {

class CameraControllerSystem : public System {
public:
    CameraControllerSystem() {
        // This system operates on entities with both these components
        setComponentMask<CameraControllerComponent, TransformComponent>();
    }

    void update(float deltaTime) override {
        for (auto entity : getEntities()) {
            auto& controller = entity->getComponent<CameraControllerComponent>();
            auto& transform = entity->getComponent<TransformComponent>();
            
            // Pass both components to the update method
            controller.update(deltaTime, transform);
        }
    }
};

} // namespace ECS
} // namespace Engine
