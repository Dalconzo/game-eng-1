#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "../include/ecs/ECSManager.h"
#include "../include/ecs/components/Transform.h"
#include "../include/ecs/components/RenderComponent.h"
#include "../include/ecs/components/Camera.h"
#include "../include/ecs/systems/RenderSystem.h"
#include "../include/ecs/systems/CameraSystem.h"
int main() {
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return -1;
    }

    // Set OpenGL version BEFORE window creation
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    #ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // Important for macOS
    #endif

    // Create window
    GLFWwindow* window = glfwCreateWindow(800, 600, "ECS Game", NULL, NULL);
    if (!window) {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    // Make context current BEFORE GLEW initialization
    glfwMakeContextCurrent(window);

    // Initialize GLEW
    GLenum err = glewInit();
    if (err != GLEW_OK) {
        std::cerr << "Failed to initialize GLEW: " << glewGetErrorString(err) << std::endl;
        return -1;
    }

    // ECS Setup
    ECSManager ecs;
    Entity player = ecs.createEntity();
    Transform& initialPlayerTransform = ecs.addComponent<Transform>(player);
    initialPlayerTransform.posX = 0.0f;
    initialPlayerTransform.posY = 0.0f;
    initialPlayerTransform.posZ = 0.0f;
    initialPlayerTransform.rotX = 0.0f;
    initialPlayerTransform.rotY = 0.0f;
    initialPlayerTransform.rotZ = 0.0f;
    initialPlayerTransform.scaleX = 1.0f;
    initialPlayerTransform.scaleY = 1.0f;
    initialPlayerTransform.scaleZ = 1.0f;
    ecs.addComponent<RenderComponent>(player);

    Entity camera = ecs.createEntity();
    Transform& camTransform = ecs.addComponent<Transform>(camera);
    camTransform.posX = 0.0f;
    camTransform.posY = 2.0f;
    camTransform.posZ = -5.0f;
    camTransform.rotX = 0.0f;
    camTransform.rotY = 0.0f;
    camTransform.rotZ = 0.0f;
    camTransform.scaleX = 1.0f;
    camTransform.scaleY = 1.0f;
    camTransform.scaleZ = 1.0f;
    Camera& cam = ecs.addComponent<Camera>(camera);
    cam.fov = 60.0f;
    cam.aspect = 800.0f / 600.0f;
    cam.nearPlane = 0.1f;
    cam.farPlane = 100.0f;

    // Register Systems
    ecs.registerSystem(std::make_unique<CameraSystem>(player));

    // In main.cpp
    if (glewInit() != GLEW_OK) {
        std::cerr << "Failed to initialize GLEW" << std::endl;
        return -1;
    }
    auto renderSystem = std::make_unique<RenderSystem>();
    renderSystem->init(window);
    ecs.registerSystem(std::move(renderSystem));

    // Main Loop
    while (!glfwWindowShouldClose(window)) {
        // Fetch the Transform component fresh each frame
        Transform* playerTransform = ecs.getComponent<Transform>(player);
        if (playerTransform) {
            playerTransform->posX += 0.01f; // Line 77: Move right slowly
        } else {
            std::cerr << "Failed to retrieve player Transform component!" << std::endl;
            break;
        }
        ecs.update();
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}