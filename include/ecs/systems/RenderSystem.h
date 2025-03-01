#pragma once
#include "../System.h"
#include "../components/TransformComponent.h"
#include "../components/MeshRendererComponent.h"
#include "../components/CameraComponent.h"
#include "rendering/shader.h"
#include "rendering/window.h"
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
        std::cout << "Starting render cycle" << std::endl;
        // Skip rendering if no camera is available
        if (!m_activeCamera || !m_activeCamera->hasComponent<CameraComponent>()) {
            std::cout << "Active camera: null" << std::endl;
            return;
        }

        std::cout << "Active camera: found" << std::endl;
        
        // Get the camera component
        auto& camera = m_activeCamera->getComponent<CameraComponent>();
        
        // Clear the screen
        camera.clear();
        
        std::cout << "Setting shader uniforms" << std::endl;
        // Set up view and projection matrices
        m_defaultShader->use();
        m_defaultShader->setMat4("view", camera.getViewMatrix().toGLM());
        m_defaultShader->setMat4("projection", camera.getProjectionMatrix().toGLM());
        
        std::cout << "Beginning entity rendering" << std::endl;
        // Render all entities with renderers
        for (auto entity : getEntities()) {
            if (entity->hasComponent<MeshRendererComponent>()) {
                std::cout << "Rendering entity: " << entity->getID() << std::endl;
                auto& renderer = entity->getComponent<MeshRendererComponent>();
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
        if (!m_ecsManager) {
            std::cout << "Error: ECSManager reference is null" << std::endl;
            return nullptr;
        }
        std::cout << "Finding main camera" << std::endl;
        auto entities = getManager()->getAllEntities();
        std::cout << "Got " << entities.size() << " entities" << std::endl;
        
        for (auto& entity : entities) {
            if (!entity) {
                std::cout << "Warning: Null entity in entity list" << std::endl;
                continue;
            }
            
            if (!entity->isActive()) {
                continue;
            }
            
            // Check for camera component safely
            std::cout << "Checking entity " << entity->getID() << std::endl;
            if (entity->hasComponent<CameraComponent>()) {
                std::cout << "Entity has camera component" << std::endl;
                auto& camera = entity->getComponent<CameraComponent>();
                if (camera.isMain()) {
                    std::cout << "Found main camera" << std::endl;
                    return entity;
                }
            }
        }
        std::cout << "No main camera found" << std::endl;
        return nullptr;
    }
};

} // namespace ECS
} // namespace Engine
