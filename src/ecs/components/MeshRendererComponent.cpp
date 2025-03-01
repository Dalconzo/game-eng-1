#include "ecs/components/MeshRendererComponent.h"
#include "ecs/components/TransformComponent.h"
#include "ecs/Entity.h"
#include <iostream>

namespace Engine {
namespace ECS {

void MeshRendererComponent::init() {
    // Optional initialization
}

void MeshRendererComponent::update(float deltaTime) {
    // Usually empty for renderers, as they update during the render phase
}

void MeshRendererComponent::render(engine::rendering::Shader& shader) {
    std::cout << "MeshRenderer: setting up rendering" << std::endl;

    if (!m_model || !isActive()) {
        return;
    }
    
    // Get the transform component
    if (!getOwner()->hasComponent<TransformComponent>()) {
        std::cout << "ERROR: Entity missing TransformComponent" << std::endl;
        return;
    }
    
    const auto& transform = getOwner()->getComponent<TransformComponent>();
    
    // Set the model matrix in the shader
    std::cout << "Setting model matrix for entity ID: " << getOwner()->getID() << std::endl;
    shader.setMat4("model", transform.getWorldMatrix().toGLM());
    
    // Apply material if available
    if (m_material) {
        std::cout << "Applying material" << std::endl;
        m_material->apply(shader);
    } else {
        std::cout << "WARNING: No material assigned" << std::endl;
    }
    
    // Render the model
    std::cout << "Rendering model" << std::endl;
    m_model->render(shader);
}

} // namespace ECS
} // namespace Engine
