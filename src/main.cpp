#include "rendering/window.h"
#include "core/game_loop.h"
#include <iostream>
#include <GL/glew.h>  // We'll need GLEW for OpenGL extension loading
#include <GLFW/glfw3.h>
#include <string>
#include <sstream>

// Global variables for this test
float rotation = 0.0f;

int main() {
    try {
        // Create a window
        engine::rendering::Window window(800, 600, "Game Engine - Test Window");
        
        // Initialize GLEW after we have a valid OpenGL context
        if (glewInit() != GLEW_OK) {
            std::cerr << "Failed to initialize GLEW" << std::endl;
            return -1;
        }
        
        // Print OpenGL version
        std::cout << "OpenGL Version: " << glGetString(GL_VERSION) << std::endl;
        
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
            
            // In a real implementation, we would render more complex objects
            // For now, we'll just make the clearing color pulse based on rotation
            // to visually show that our time-based updates are working
            float r = (sin(rotation * 0.01f) + 1.0f) * 0.5f;
            float g = (cos(rotation * 0.01f) + 1.0f) * 0.5f;
            glClearColor(r, g, 0.5f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT);
            
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
