#pragma once
#include "../System.h"
#include "../components/TransformComponent.h"
#include "../components/MeshRendererComponent.h"
#include "../components/CameraComponent.h"
#include "rendering/shader.h"
#include "rendering/window.h"
#include "core/resource_manager.h"
#include <vector>
#include <memory>
#include <iostream>

namespace Engine {
namespace ECS {
    class ECSManager;
    class Entity;

class RenderSystem : public System {
public:
    void setManager(ECSManager* manager) { m_ecsManager = manager; }

    void setManagerInternal(ECSManager* manager) override {
        setManager(manager);
    }

    ECSManager* getManager() { return m_ecsManager; }

    RenderSystem() {
        // Register which components this system operates on
        setComponentMask<TransformComponent, MeshRendererComponent>();
    }

    virtual void init() override {
        std::cout << "RenderSystem: Initializing\n";
        // Load the default shader
        m_defaultShader = ECSManager::loadShader(
            "defaultShader", 
            "shaders/default.vert", 
            "shaders/default.frag"
        );
        if (!m_defaultShader) {
            std::cerr << "RenderSystem: Failed to load shader\n";
        }
    }

    virtual void update(float deltaTime) override {
        // Find the main camera
        m_activeCamera = findMainCamera();
    }

    virtual void render() override {
        std::cout << "RenderSystem: Starting render cycle" << std::endl;
        // Skip rendering if no camera is available
        if (!m_activeCamera) {
            std::cerr << "RenderSystem: No main camera found for rendering\n";
            return;
        }

        std::cout << "RenderSystem: Rendering with camera from entity " << m_activeCamera->getID() << std::endl;
        
        // Get the camera component
        auto& camera = m_activeCamera->getComponent<CameraComponent>();
        
        // Clear the screen
        camera.clear();
        
        // Check if shader is available
        if (!m_defaultShader) {
            std::cerr << "RenderSystem: No shader available for rendering\n";
            return;
        }
        
        std::cout << "RenderSystem: Setting shader uniforms" << std::endl;
        // Set up view and projection matrices
        m_defaultShader->use();
        m_defaultShader->setMat4("view", camera.getViewMatrix().toGLM());
        m_defaultShader->setMat4("projection", camera.getProjectionMatrix().toGLM());
        
        std::cout << "RenderSystem: Beginning entity rendering" << std::endl;
        // Render all entities with renderers
        for (auto entity : getEntities()) {
            if (entity->hasComponent<MeshRendererComponent>() && 
                entity->hasComponent<TransformComponent>()) {
                std::cout << "RenderSystem: Rendering entity " << entity->getID() << std::endl;
                auto& renderer = entity->getComponent<MeshRendererComponent>();
                auto& transform = entity->getComponent<TransformComponent>();
                m_defaultShader->setMat4("model", transform.getWorldMatrix().toGLM());
                renderer.render(*m_defaultShader);
            }
        }
    }

private:
    std::shared_ptr<engine::rendering::Shader> m_defaultShader;
    Entity* m_activeCamera = nullptr;
    ECSManager* m_ecsManager = nullptr;
    void renderEntity(Entity* entity, const Math::Matrix4x4& worldMatrix);
    
    Entity* findMainCamera() {
        auto entities = getManager()->getAllEntities();
        std::cout << "RenderSystem: Searching through " << entities.size() << " entities for main camera" << std::endl;
        
        for (auto& entity : entities) {
            std::cout << "RenderSystem: Checking entity " << entity->getID() 
                      << " (active: " << entity->isActive()
                      << ", hasCamera: " << entity->hasComponent<CameraComponent>() << ")" << std::endl;
                      
            if (entity && entity->isActive() && 
                entity->hasComponent<CameraComponent>()) {
                    
                auto& camera = entity->getComponent<CameraComponent>();
                std::cout << "RenderSystem: Found camera component, isMain: " << camera.isMain() << std::endl;
                if (camera.isMain()) {
                    std::cout << "RenderSystem: Found main camera on entity " << entity->getID() << std::endl;
                    return entity;
                }
            }
        }
        std::cerr << "RenderSystem: No main camera found" << std::endl;
        return nullptr;
    }
};

} // namespace ECS
} // namespace Engine
