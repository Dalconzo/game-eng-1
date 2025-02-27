#pragma once
#include "../System.h"
#include "../components/TransformComponent.h"

namespace Engine {
namespace ECS {

class RenderSystem : public System {
public:
    RenderSystem() {
        // Register which components this system operates on
        setComponentMask<TransformComponent>();
    }

    virtual void init() override {
        // Initialize renderer
    }

    virtual void update(float deltaTime) override {
        // Update animations, etc.
    }

    virtual void render() override {
        for (auto entity : getEntities()) {
            auto& transform = entity->getComponent<TransformComponent>();
            
            // Get world matrix
            Math::Matrix4x4 worldMatrix = transform.getWorldMatrix();
            
            // Render the entity (would connect to your graphics API)
            // This is a placeholder - you'd need to implement actual rendering
            // based on your graphics API choice (OpenGL, DirectX, etc.)
            renderEntity(entity, worldMatrix);
        }
    }

private:
    void renderEntity(Entity* entity, const Math::Matrix4x4& worldMatrix) {
        // Placeholder for actual rendering code
        // You would use your graphics API here to render the entity
    }
};

} // namespace ECS
} // namespace Engine
