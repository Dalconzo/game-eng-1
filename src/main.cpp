#include <GL/glew.h>
#include "rendering/window.h"
#include "ecs/ECSManager.h"
#include "ecs/components/TransformComponent.h"
#include "ecs/components/MeshRendererComponent.h"
#include "ecs/components/CameraComponent.h"
#include "ecs/systems/RenderSystem.h"
#include "core/resource_manager.h"
#include "rendering/primitive_builder.h"
#include <iostream>
#include <memory>

using namespace Engine;
using namespace Engine::ECS;
using namespace Engine::Math;

// Create a sample scene with entities
void createScene(ECSManager& manager) {
    // Create a camera entity
    auto cameraEntity = manager.createEntity();
    cameraEntity->addComponent<TransformComponent>(Vector3(0, 2, 5));
    auto& camera = cameraEntity->addComponent<CameraComponent>();
    camera.setPerspective(45.0f, 800.0f / 600.0f, 0.1f, 1000.0f);
    camera.setMain(true);
    
    // Create a cube entity
    auto cubeEntity = manager.createEntity();
    cubeEntity->addComponent<TransformComponent>(Vector3(0, 0, 0));
    auto& renderer = cubeEntity->addComponent<MeshRendererComponent>();
    
    // Load a model and material
    auto model = ECSManager::loadModel("models/cube.obj");
    auto material = ECSManager::createMaterial("cubeMaterial");
    
    // Set up the renderer
    renderer.setModel(model);
    renderer.setMaterial(material);
    
    // Create some child objects
    auto childEntity = manager.createEntity();
    childEntity->addComponent<TransformComponent>(Vector3(2, 0, 0));
    auto& childRenderer = childEntity->addComponent<MeshRendererComponent>();
    childRenderer.setModel(model);
    
    // Create parent-child relationship
    childEntity->setParent(cubeEntity);
}

int main() {
    try {
        engine::core::ResourceManager::init("assets");
        // Create a window
        auto window = std::make_unique<engine::rendering::Window>(800, 600, "ECS Game Engine");
        
        // Initialize GLEW
        if (glewInit() != GLEW_OK) {
            std::cerr << "Failed to initialize GLEW" << std::endl;
            return -1;
        }
        
        // Enable depth testing
        glEnable(GL_DEPTH_TEST);
        
        // Create and initialize ECS manager
        ECSManager ecsManager;
        ecsManager.initialize(window.get());
        
        // Register systems
        ecsManager.registerSystem<RenderSystem>();
        
        // Create scene entities
        createScene(ecsManager);
        
        // Run the game loop - this contains the update/render loop
        ecsManager.runGameLoop();
        
        // Cleanup is handled by destructors and ECSManager::shutdown()
        return 0;
    }
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return -1;
    }
}
