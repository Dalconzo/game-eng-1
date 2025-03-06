#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "rendering/window.h"
#include "ecs/ECSManager.h"
#include "ecs/systems/RenderSystem.h"
#include "core/debug/debug_utils.h"
#include "core/debug/logger.h"
#include "ecs/components/OrbitCameraController.h"
#include "ecs/components/CameraControllerComponent.h"
#include "ecs/systems/CameraControllerSystem.h"

void createScene(Engine::ECS::ECSManager& manager);
void diagnoseCameraIssue(Engine::ECS::ECSManager& manager);

void diagnoseCameraIssue(Engine::ECS::ECSManager& manager) {
    // First create a new test camera
    std::cout << "Creating diagnostic camera entity..." << std::endl;
    auto testCamera = manager.createEntity();
    std::cout << "Test camera entity ID: " << testCamera->getID() << std::endl;
    
    testCamera->addComponent<Engine::ECS::TransformComponent>();
    auto& camera = testCamera->addComponent<Engine::ECS::CameraComponent>();
    camera.setMain(true);
    
    // Now check all entities
    std::cout << "Checking all entities after diagnostic camera..." << std::endl;
    auto entities = manager.getAllEntities();
    for (auto entity : entities) {
        std::cout << "Entity ID: " << entity->getID() 
                  << " hasCamera: " << entity->hasComponent<Engine::ECS::CameraComponent>()
                  << std::endl;
        
        if (entity->hasComponent<Engine::ECS::CameraComponent>()) {
            auto& cameraComp = entity->getComponent<Engine::ECS::CameraComponent>();
            std::cout << "  Camera isMain: " << cameraComp.isMain() << std::endl;
        }
    }
}

void runTests(Engine::ECS::ECSManager& ecsManager) {
    std::cout << "\n===== DIAGNOSTIC TEST SUITE =====\n" << std::endl;
    
    // Test 1: Camera entity creation
    std::cout << "\n----- Test: Camera Entity Creation -----" << std::endl;
    diagnoseCameraIssue(ecsManager);
    
    // Add more targeted tests as needed
    std::cout << "\n===== END DIAGNOSTIC TESTS =====\n" << std::endl;
}


void testTransformMatrices() {
    // Test local matrix calculation
    Engine::ECS::TransformComponent transform;
    
    // Test 1: Default position (0,0,0)
    Engine::Math::Matrix4x4 matrix1 = transform.getLocalMatrix();
    std::cout << "Test 1 - Identity Matrix: " 
              << matrix1(0,0) << "," << matrix1(1,1) << "," 
              << matrix1(2,2) << "," << matrix1(3,3) << std::endl;
    std::cout << "Position elements: " 
              << matrix1(0,3) << "," << matrix1(1,3) << "," << matrix1(2,3) << std::endl;
    
    // Test 2: Move to position (2,3,4)
    transform.setPosition(Engine::Math::Vector3(2, 3, 4));
    Engine::Math::Matrix4x4 matrix2 = transform.getLocalMatrix();
    std::cout << "Test 2 - Translated Matrix: " 
              << matrix2(0,0) << "," << matrix2(1,1) << "," 
              << matrix2(2,2) << "," << matrix2(3,3) << std::endl;
    std::cout << "Position elements: " 
              << matrix2(0,3) << "," << matrix2(1,3) << "," << matrix2(2,3) << std::endl;

    // Add to your test function
    glm::mat4 glmMat = matrix2.toGLM();
    std::cout << "Full GLM matrix:" << std::endl;
    for (int i = 0; i < 4; i++) {
        std::cout << "  ";
        for (int j = 0; j < 4; j++) {
            std::cout << glmMat[j][i] << " ";
        }
        std::cout << std::endl;
    }
    
    // Test 3: Check matrix multiplication in Matrix4x4
    Engine::Math::Matrix4x4 translationMatrix = 
        Engine::Math::Matrix4x4::createTranslation(Engine::Math::Vector3(1, 2, 3));
    std::cout << "Translation Matrix - Position elements: " 
              << translationMatrix(0,3) << "," << translationMatrix(1,3) << "," 
              << translationMatrix(2,3) << std::endl;
}

void testViewMatrix() {
    // Create a camera position
    Engine::Math::Vector3 position(0, 2, 5);
    Engine::Math::Vector3 target(0, 0, 0);  // Looking at origin
    Engine::Math::Vector3 up(0, 1, 0);      // Y-up
    
    // Create view matrix
    Engine::Math::Matrix4x4 viewMatrix = Engine::Math::Matrix4x4::createLookAt(position, target, up);
    
    // Convert to GLM for inspection
    glm::mat4 glmView = viewMatrix.toGLM();
    
    // Print full matrix
    std::cout << "Test view matrix:" << std::endl;
    for (int i = 0; i < 4; i++) {
        std::cout << "  ";
        for (int j = 0; j < 4; j++) {
            std::cout << glmView[j][i] << " ";
        }
        std::cout << std::endl;
    }
}

void testCameraViewMatrix() {
    // Create a simple entity with transform and camera components
    auto manager = std::make_unique<Engine::ECS::ECSManager>();
    auto entity = manager->createEntity();
    
    // Set position to match our camera in the scene
    entity->addComponent<Engine::ECS::TransformComponent>(Engine::Math::Vector3(0, 2, 5));
    auto& camera = entity->addComponent<Engine::ECS::CameraComponent>();
    
    // Force recalculation of view matrix
    camera.m_viewDirty = true;
    
    // Get view matrix
    Engine::Math::Matrix4x4 viewMatrix = camera.getViewMatrix();
    
    // Convert to GLM for inspection
    glm::mat4 glmView = viewMatrix.toGLM();
    
    // Print full matrix
    std::cout << "Camera component view matrix:" << std::endl;
    for (int i = 0; i < 4; i++) {
        std::cout << "  ";
        for (int j = 0; j < 4; j++) {
            std::cout << glmView[j][i] << " ";
        }
        std::cout << std::endl;
    }
}


int main() {
    try {
        // Initialize debug utilities first
        engine::core::debug::initDebugUtils();
        
        LOG_INFO("Application starting");
        
        // Create a window
        LOG_DEBUG("Creating window");
        auto window = std::make_unique<engine::rendering::Window>(800, 600, "ECS Game Engine");
        LOG_INFO("Window created successfully");
        
        // Initialize GLEW
        LOG_DEBUG("Initializing GLEW");
        if (glewInit() != GLEW_OK) {
            LOG_FATAL("Failed to initialize GLEW");
            return -1;
        }
        LOG_INFO("GLEW initialized successfully");

        if (glewInit() != GLEW_OK) {
            LOG_FATAL("Failed to initialize GLEW");
            return -1;
        }

        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

        // Enable depth testing
        glEnable(GL_DEPTH_TEST);

        // Add these lines here
        glEnable(GL_CULL_FACE);
        glCullFace(GL_BACK);

        // Run our test function here
        LOG_INFO("Running view matrix tests");
        testViewMatrix();
        testCameraViewMatrix();
        
        // Enable depth testing
        glEnable(GL_DEPTH_TEST);
        
        // Create and initialize ECS manager
        LOG_DEBUG("Creating ECS manager");
        Engine::ECS::ECSManager ecsManager;
        
        LOG_DEBUG("Initializing ECS manager");
        ecsManager.initialize(window.get());
        LOG_INFO("ECS manager initialized successfully");
        
        // Register systems with logging checkpoints
        LOG_DEBUG("Registering RenderSystem");
        ecsManager.registerSystem<Engine::ECS::RenderSystem>();

        // In main.cpp after registering the RenderSystem
        ecsManager.registerSystem<Engine::ECS::CameraControllerSystem>();

        
        // Create scene entities with logging checkpoints
        LOG_DEBUG("Creating scene");
        createScene(ecsManager);
        LOG_INFO("Scene created successfully");

        // Run diagnostics
        // runTests(ecsManager);
        
        // Run the game loop with additional logging
        LOG_INFO("Starting game loop");
        ecsManager.runGameLoop();
        
        return 0;
    }
    catch (const std::exception& e) {
        LOG_FATAL(std::string("Error: ") + e.what());
        return -1;
    }
}

void createScene(Engine::ECS::ECSManager& manager) {
    // Create a camera entity
    auto cameraEntity = manager.createEntity();
    std::cout << "Created camera entity with ID: " << cameraEntity->getID() << std::endl;
    
    cameraEntity->addComponent<Engine::ECS::TransformComponent>(Engine::Math::Vector3(0, 2, 5));
    std::cout << "Added TransformComponent to camera entity" << std::endl;
    
    auto& camera = cameraEntity->addComponent<Engine::ECS::CameraComponent>();
    std::cout << "Added CameraComponent to camera entity" << std::endl;
    
    camera.setPerspective(45.0f, 800.0f / 600.0f, 0.1f, 1000.0f);
    camera.setMain(true);

    auto& cameraController = cameraEntity->addComponent<Engine::ECS::CameraControllerComponent>();
    cameraController.setMoveSpeed(5.0f);

    cameraEntity->addComponent<Engine::ECS::OrbitCameraController>();
    
    // Add diagnostic output to confirm camera properties
    std::cout << "Camera position: (" 
              << cameraEntity->getComponent<Engine::ECS::TransformComponent>().getPosition().x
              << ", " << cameraEntity->getComponent<Engine::ECS::TransformComponent>().getPosition().y
              << ", " << cameraEntity->getComponent<Engine::ECS::TransformComponent>().getPosition().z << ")\n";
    std::cout << "Camera is main: " << camera.isMain() << "\n";
    
    // Create a cube entity
    auto cubeEntity = manager.createEntity();
    cubeEntity->addComponent<Engine::ECS::TransformComponent>(Engine::Math::Vector3(0, 0, 0));
    auto& renderer = cubeEntity->addComponent<Engine::ECS::MeshRendererComponent>();
    
    // Load a model and material
    auto material = Engine::ECS::ECSManager::createMaterial("cubeMaterial");
    auto model = Engine::ECS::ECSManager::loadModel("models/cube.obj");
    
    
    // Set up the renderer
    renderer.setModel(model);
    renderer.setMaterial(material);
    
    // Create some child objects
    auto childEntity = manager.createEntity();
    childEntity->addComponent<Engine::ECS::TransformComponent>(Engine::Math::Vector3(2, 0, 0));
    auto& childRenderer = childEntity->addComponent<Engine::ECS::MeshRendererComponent>();
    childRenderer.setModel(model);
    
    // Create parent-child relationship
    childEntity->setParent(cubeEntity);
}
