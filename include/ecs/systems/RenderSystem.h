#pragma once
#include "../System.h"
#include "../components/TransformComponent.h"
#include "../components/MeshRendererComponent.h"
#include "../components/CameraComponent.h"
#include "rendering/shader.h"
#include "rendering/window.h"
#include <vector>
#include <memory>

namespace Engine {
namespace ECS {
    class ECSManager;
    class Entity;

class RenderSystem : public System {
private:
    std::shared_ptr<engine::rendering::Shader> m_defaultShader;
    Entity* m_activeCamera = nullptr;
    ECSManager* m_ecsManager = nullptr;
    void renderEntity(Entity* entity, const Math::Matrix4x4& worldMatrix);
    
public:
    void setManager(ECSManager* manager) { m_ecsManager = manager; }
    ECSManager* getManager() { return m_ecsManager; }

    RenderSystem() {
        // Register which components this system operates on
        setComponentMask<TransformComponent, MeshRendererComponent>();
    }

    virtual void init() override {
        // Load the default shader
        m_defaultShader = ECSManager::loadShader(
            "defaultShader", 
            "shaders/default.vert", 
            "shaders/default.frag"
        );
    }

    virtual void update(float deltaTime) override {
        // Find the main camera
        m_activeCamera = findMainCamera();
    }

    virtual void render() override {
        // Skip rendering if no camera is available
        if (!m_activeCamera || !m_activeCamera->hasComponent<CameraComponent>()) {
            return;
        }
        
        // Get the camera component
        auto& camera = m_activeCamera->getComponent<CameraComponent>();
        
        // Clear the screen
        camera.clear();
        
        // Set up view and projection matrices
        m_defaultShader->use();
        m_defaultShader->setMat4("view", camera.getViewMatrix().toGLM());
        m_defaultShader->setMat4("projection", camera.getProjectionMatrix().toGLM());
        
        // Render all entities with renderers
        for (auto entity : getEntities()) {
            if (entity->hasComponent<MeshRendererComponent>()) {
                auto& renderer = entity->getComponent<MeshRendererComponent>();
                renderer.render(*m_defaultShader);
            }
        }
    }

private:
    Entity* findMainCamera() {
        // This could be optimized with a direct reference to camera entities
        for (auto& entity : getManager()->getAllEntities()) {
            if (entity && entity->isActive() && 
                entity->hasComponent<CameraComponent>()) {
                    
                auto& camera = entity->getComponent<CameraComponent>();
                if (camera.isMain()) {
                    return entity;
                }
            }
        }
        return nullptr;
    }
};

} // namespace ECS
} // namespace Engine
