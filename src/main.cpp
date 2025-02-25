#include "rendering/window.h"
#include "rendering/shader.h"
#include "rendering/mesh.h"
#include "rendering/primitive_builder.h"
#include "core/game_loop.h"
#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <string>
#include <sstream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "core/resource_manager.h"
#include <filesystem>

// Global variables for this test
float rotation = 0.0f;

int main() {
    try {
        // Create a window
        // Initialize the resource manager with automatic root detection
        engine::core::ResourceManager::init();
        engine::rendering::Window window(800, 600, "Game Engine - Test Window");
        
        // Initialize GLEW
        if (glewInit() != GLEW_OK) {
            std::cerr << "Failed to initialize GLEW" << std::endl;
            return -1;
        }
        
        // Print OpenGL version
        std::cout << "OpenGL Version: " << glGetString(GL_VERSION) << std::endl;
        
        // Create and compile our shader
        engine::rendering::Shader shader;
        if (!shader.loadFromFiles(
                engine::core::ResourceManager::resolvePath("assets/shaders/basic.vert"),
                engine::core::ResourceManager::resolvePath("assets/shaders/basic.frag"))) {
            std::cerr << "Failed to load shaders" << std::endl;
            return -1;
        }
        
        // Create our primitive
        std::unique_ptr<engine::rendering::Mesh> cube = 
            engine::rendering::PrimitiveBuilder::createCube();
        
        // Enable depth testing
        glEnable(GL_DEPTH_TEST);
        
        // Create camera view and projection matrices
        glm::mat4 view = glm::lookAt(
            glm::vec3(0.0f, 0.0f, 3.0f),
            glm::vec3(0.0f, 0.0f, 0.0f),
            glm::vec3(0.0f, 1.0f, 0.0f)
        );
        glm::mat4 projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);
        
        // Create the game loop
        engine::core::GameLoop gameLoop;
        
        // Set up callbacks
        gameLoop.setUpdateCallback([&](double deltaTime) {
            // Process input
            if (glfwGetKey(window.getNativeWindow(), GLFW_KEY_ESCAPE) == GLFW_PRESS) {
                window.close();
                gameLoop.stop();
            }
            
            // Update game state - rotate something based on delta time
            rotation += static_cast<float>(deltaTime) * 50.0f; // 50 degrees per second
            
            // Update window title with FPS
            std::stringstream ss;
            ss << "Game Engine - FPS: " << gameLoop.getTimeManager().getFPS();
            glfwSetWindowTitle(window.getNativeWindow(), ss.str().c_str());
        });
        
        gameLoop.setFixedUpdateCallback([](double fixedTimeStep) {
            // Physics updates would go here
        });
        
        gameLoop.setRenderCallback([&]() {
            // Clear screen
            glClearColor(0.392f, 0.584f, 0.929f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            
            // Use our shader
            shader.use();
            
            // Create model matrix
            glm::mat4 model = glm::rotate(glm::mat4(1.0f), glm::radians(rotation), glm::vec3(0.5f, 1.0f, 0.0f));
            
            // Set uniforms
            shader.setMat4("model", model);
            shader.setMat4("view", view);
            shader.setMat4("projection", projection);
            
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
