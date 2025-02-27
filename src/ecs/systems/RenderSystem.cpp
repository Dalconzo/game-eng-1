#include "include/ecs/systems/RenderSystem.h"
#include "include/ecs/components/TransformComponent.h"
#include <iostream> // For debug output during prototype

namespace Engine {
namespace ECS {

RenderSystem::RenderSystem() {
    // Register which components this system operates on
    setComponentMask<TransformComponent>();
}

void RenderSystem::init() {
    // Initialize renderer resources
    std::cout << "RenderSystem initialized" << std::endl;
}

void RenderSystem::update(float deltaTime) {
    // Update animations or other time-dependent visual elements
    // This would include things like shader parameter updates, particle systems, etc.
    
    // For the prototype, nothing to do here yet
}

void RenderSystem::render() {
    // In a full implementation, we would:
    // 1. Sort entities by material/shader for batching
    // 2. Set up camera and view/projection matrices
    // 3. Configure global rendering state
    
    std::cout << "RenderSystem rendering " << getEntities().size() << " entities" << std::endl;
    
    for (auto entity : getEntities()) {
        auto& transform = entity->getComponent<TransformComponent>();
        
        // Get world matrix
        Math::Matrix4x4 worldMatrix = transform.getWorldMatrix();
        
        // Render the entity
        renderEntity(entity, worldMatrix);
    }
}

void RenderSystem::renderEntity(Entity* entity, const Math::Matrix4x4& worldMatrix) {
    // In a full implementation, this would connect to your graphics API
    
    // For the prototype, just print debug info
    std::cout << "Rendering entity " << entity->getID() 
              << " at position (" 
              << worldMatrix(0, 3) << ", " 
              << worldMatrix(1, 3) << ", " 
              << worldMatrix(2, 3) << ")" 
              << std::endl;
    
    // In an actual implementation, we would:
    // 1. Bind entity's material and shader
    // 2. Set shader uniforms (world matrix, etc.)
    // 3. Bind mesh data (VAO/VBO)
    // 4. Issue draw call (glDrawElements or equivalent)
}

} // namespace ECS
} // namespace Engine
