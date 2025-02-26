#include <GL/glew.h>
#include "rendering/window.h"
#include "rendering/shader.h"
#include "rendering/mesh.h"
#include "rendering/primitive_builder.h"
#include "camera/camera_controller.h"
#include "camera/third_person_camera.h"
#include "core/game_loop.h"
#include "core/resource_manager.h"
#include <iostream>
#include <GLFW/glfw3.h>
#include <string>
#include <sstream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <memory>

// Global variables for camera control
std::shared_ptr<engine::rendering::CameraController> g_cameraController;

// GLFW callback functions
void mouseCallback(GLFWwindow* window, double xpos, double ypos) {
    g_cameraController->processMouseMovement(static_cast<float>(xpos), static_cast<float>(ypos));
}

void scrollCallback(GLFWwindow* window, double xoffset, double yoffset) {
    g_cameraController->processMouseScroll(static_cast<float>(yoffset));
}

void framebufferSizeCallback(GLFWwindow* window, int width, int height) {
    // Update viewport
    glViewport(0, 0, width, height);
    
    // Update camera aspect ratio
    if (g_cameraController && g_cameraController->getCamera()) {
        float aspectRatio = static_cast<float>(width) / static_cast<float>(height);
        g_cameraController->getCamera()->setPerspective(45.0f, aspectRatio, 0.1f, 1000.0f);
    }
}

int main() {
    try {
        // Initialize the resource manager
        engine::core::ResourceManager::init();
        
        // Create a window
        engine::rendering::Window window(800, 600, "Game Engine - Camera Demo");
        
        // Initialize GLEW
        if (glewInit() != GLEW_OK) {
            std::cerr << "Failed to initialize GLEW" << std::endl;
            return -1;
        }
        
        // Print OpenGL version
        std::cout << "OpenGL Version: " << glGetString(GL_VERSION) << std::endl;
        
        // Create and initialize the camera
        auto camera = std::make_shared<engine::rendering::ThirdPersonCamera>();
        camera->setPerspective(45.0f, 800.0f / 600.0f, 0.1f, 1000.0f);
        camera->setTargetPosition(glm::vec3(0.0f, 0.0f, 0.0f));
        camera->setDistance(5.0f);
        
        // Create camera controller
        g_cameraController = std::make_shared<engine::rendering::CameraController>(window.getNativeWindow());
        g_cameraController->setCamera(camera);
        
        // Set up GLFW callbacks
        glfwSetCursorPosCallback(window.getNativeWindow(), mouseCallback);
        glfwSetScrollCallback(window.getNativeWindow(), scrollCallback);
        glfwSetFramebufferSizeCallback(window.getNativeWindow(), framebufferSizeCallback);
        
        // Enable mouse capture for FPS-style control
        // Comment this out for third-person orbiting without cursor capture
        // glfwSetInputMode(window.getNativeWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        
        // Create and compile our shader
        engine::rendering::Shader shader;
        if (!shader.loadFromFiles(
                engine::core::ResourceManager::resolvePath("assets/shaders/basic.vert"),
                engine::core::ResourceManager::resolvePath("assets/shaders/basic.frag"))) {
            std::cerr << "Failed to load shaders" << std::endl;
            return -1;
        }
        
        // Create a test cube
        std::unique_ptr<engine::rendering::Mesh> cube = 
            engine::rendering::PrimitiveBuilder::createCube();
        
        // Enable depth testing
        glEnable(GL_DEPTH_TEST);
        
        // Create the game loop
        engine::core::GameLoop gameLoop;
        
        // Set up callbacks
        gameLoop.setUpdateCallback([&](double deltaTime) {
            // Process input for closing the window
            if (glfwGetKey(window.getNativeWindow(), GLFW_KEY_ESCAPE) == GLFW_PRESS) {
                window.close();
                gameLoop.stop();
            }
            
            // Process camera input
            g_cameraController->processInput(static_cast<float>(deltaTime));
            
            // Update window title with FPS
            std::stringstream ss;
            ss << "Camera Demo - FPS: " << gameLoop.getTimeManager().getFPS();
            glfwSetWindowTitle(window.getNativeWindow(), ss.str().c_str());
        });
        
        gameLoop.setRenderCallback([&]() {
            // Clear screen
            glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            
            // Use our shader
            shader.use();
            
            // Create model matrix
            glm::mat4 model = glm::mat4(1.0f);
            
            // Set uniforms using our camera
            shader.setMat4("model", model);
            shader.setMat4("view", camera->getViewMatrix());
            shader.setMat4("projection", camera->getProjectionMatrix());
            
            // Render the cube
            cube->render(shader);
            
            // Swap buffers and poll events
            window.swapBuffers();
            window.pollEvents();
        });
        
        // Start the game loop
        while (!window.shouldClose()) {
            gameLoop.runFrame();
        }
        
        return 0;
    }
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return -1;
    }
}
