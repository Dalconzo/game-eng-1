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
    
    // Get the entity's transform component
    if (getOwner()->hasComponent<TransformComponent>()) {
        const auto& transform = getOwner()->getComponent<TransformComponent>();
        
        // Get the world transformation matrix
        Math::Matrix4x4 worldMatrix = transform.getWorldMatrix();
        
        // Debug output to verify matrix values
        glm::mat4 glmModel = worldMatrix.toGLM();
        std::cout << "Model matrix for entity ID: " << getOwner()->getID() << std::endl;
        for (int i = 0; i < 4; i++) {
            std::cout << "  ";
            for (int j = 0; j < 4; j++) {
                std::cout << glmModel[j][i] << " ";
            }
            std::cout << std::endl;
        }
        
        // Set the model matrix in the shader
        shader.setMat4("model", glmModel);
        
        // Apply material if available
        if (m_material) {
            m_material->apply(shader);
        } else {
            std::cout << "WARNING: No material assigned" << std::endl;
        }
        
        // Render the model
        m_model->render(shader);
    }
}
} // namespace ECS
} // namespace Engine
