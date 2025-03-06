#include "ecs/systems/RenderSystem.h"
#include "ecs/components/TransformComponent.h"
#include "ecs/ECSManager.h"
#include <iostream> // For debug output during prototype
#include "ecs/Entity.h"

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
    // Find the main camera if we don't have one already
    if (!m_activeCamera) {
        m_activeCamera = findMainCamera();
    }
    
    // Skip rendering if no camera is available
    if (!m_activeCamera || !m_activeCamera->hasComponent<CameraComponent>()) {
        std::cout << "No active camera found. Skipping render." << std::endl;
        return;
    }
    
    // Get the camera component
    auto& camera = m_activeCamera->getComponent<CameraComponent>();
    
    // Clear the screen
    camera.clear();
    
    // Enable depth testing for proper 3D rendering
    glEnable(GL_DEPTH_TEST);
    
    // Enable backface culling for performance
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    
    // Sort entities for proper rendering order (optional)
    // sortEntitiesByDistance();
    
    // Render all entities with renderers
    for (auto entity : getEntities()) {
        if (entity->hasComponent<TransformComponent>()) {
            auto& transform = entity->getComponent<TransformComponent>();
            
            // Get world matrix
            Math::Matrix4x4 worldMatrix = transform.getWorldMatrix();
            
            // Render the entity
            renderEntity(entity, worldMatrix);
        }
    }
    
    // Disable states we enabled
    glDisable(GL_CULL_FACE);
    
    // Render any post-processing effects (if any)
    // renderPostProcessing();
}

void RenderSystem::renderEntity(Entity* entity, const Math::Matrix4x4& worldMatrix) {
    // Get the mesh renderer component
    if (!entity->hasComponent<MeshRendererComponent>()) {
        return;
    }

    auto& renderer = entity->getComponent<MeshRendererComponent>();
    auto model = renderer.getModel();
    auto material = renderer.getMaterial();

    // Skip rendering if no model or material is assigned
    if (!model || !material) {
        std::cout << "Missing model or material for entity " << entity->getID() << std::endl;
        return;
    }

    // Use the shader and set the world matrix (model matrix)
    m_defaultShader->use();
    m_defaultShader->setMat4("model", worldMatrix.toGLM());

    // Set view and projection matrices from the active camera
    if (m_activeCamera && m_activeCamera->hasComponent<CameraComponent>()) {
        auto& camera = m_activeCamera->getComponent<CameraComponent>();
        m_defaultShader->setMat4("model", worldMatrix.toGLM());
        m_defaultShader->setMat4("view", camera.getViewMatrix().toGLM());
        m_defaultShader->setMat4("projection", camera.getProjectionMatrix().toGLM());

        // Check for OpenGL errors after setting uniforms
        GLenum err = glGetError();
        if (err != GL_NO_ERROR) {
            std::cout << "OpenGL error: " << err << " after setting uniforms" << std::endl;
        }
    }

    // Apply the material (sets textures, colors, etc.)
    material->apply(*m_defaultShader);

    // Render the model (this will loop through meshes and draw them)
    model->render(*m_defaultShader);

    // Check for OpenGL errors after rendering
    GLenum err = glGetError();
    if (err != GL_NO_ERROR) {
        std::cout << "OpenGL error: " << err << " after rendering model" << std::endl;
    }
}

Entity* RenderSystem::findMainCamera() {
    // Verify manager exists before using
    if (!m_ecsManager) {
        std::cout << "Error: ECSManager is null" << std::endl;
        return nullptr;
    }
    
    // Now we can safely use getAllEntities
    auto entities = m_ecsManager->getAllEntities();
    std::cout << "Got " << entities.size() << " entities" << std::endl;
    
    for (auto entity : entities) {
        std::cout << "Checking entity " << entity->getID() << std::endl;
        if (entity->hasComponent<CameraComponent>()) {
            auto& camera = entity->getComponent<CameraComponent>();
            if (camera.isMain()) {
                std::cout << "Found main camera on entity " << entity->getID() << std::endl;
                return entity;
            }
        }
    }
    
    std::cout << "No main camera found" << std::endl;
    return nullptr;
}
} // namespace ECS
} // namespace Engine


