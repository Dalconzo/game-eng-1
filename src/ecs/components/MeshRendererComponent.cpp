#include "ecs/components/MeshRendererComponent.h"
#include "ecs/components/TransformComponent.h"
#include "ecs/Entity.h"

namespace Engine {
namespace ECS {

void MeshRendererComponent::init() {
    // Optional initialization
}

void MeshRendererComponent::update(float deltaTime) {
    // Usually empty for renderers, as they update during the render phase
}

void MeshRendererComponent::render(engine::rendering::Shader& shader) {
    if (!m_model || !isActive()) {
        return;
    }
    
    // Get the transform component
    if (!getOwner()->hasComponent<TransformComponent>()) {
        return;
    }
    
    const auto& transform = getOwner()->getComponent<TransformComponent>();
    
    // Set the model matrix in the shader
    shader.setMat4("model", transform.getWorldMatrix().toGLM());
    
    // Apply material if available
    if (m_material) {
        m_material->apply(shader);
    }
    
    // Render the model
    m_model->render(shader);
}

} // namespace ECS
} // namespace Engine
