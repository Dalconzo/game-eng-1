#pragma once
#include "../Component.h"
#include "../ECSManager.h"
#include "../components/CameraComponent.h"
#include "../components/TransformComponent.h"
#include "rendering/window.h"
#include <GLFW/glfw3.h>

namespace Engine {
namespace ECS {

class OrbitCameraController : public Component {
private:
    float m_horizontalAngle = 0.0f;
    float m_verticalAngle = 30.0f;
    float m_distance = 5.0f;
    Math::Vector3 m_focusPoint = Math::Vector3(0, 0, 0);
    float m_panSpeed = 2.0f;
    float m_rotationSpeed = 0.5f;
    float m_zoomSpeed = 0.5f;
    
    bool m_isDragging = false;
    double m_lastMouseX = 0.0;
    double m_lastMouseY = 0.0;
    
    engine::rendering::Window* m_window = nullptr;
    
public:
    void init() override {
        // Get reference to camera and transform components
        if (!getOwner()->hasComponent<CameraComponent>() || 
            !getOwner()->hasComponent<TransformComponent>()) {
            std::cerr << "OrbitCameraController requires CameraComponent and TransformComponent" << std::endl;
            return;
        }
        
        // Get window from ECSManager
        m_window = getOwner()->getManager()->getWindow();
        if (!m_window) {
            std::cerr << "Could not get window reference" << std::endl;
            return;
        }
        
        // Set initial camera position
        updateCameraPosition();
        
        // Register input callbacks
        GLFWwindow* nativeWindow = m_window->getNativeWindow();
        
        // Store this component in GLFW user pointer for callbacks
        glfwSetWindowUserPointer(nativeWindow, this);
        
        // Set up mouse callbacks
        glfwSetMouseButtonCallback(nativeWindow, [](GLFWwindow* window, int button, int action, int mods) {
            auto controller = static_cast<OrbitCameraController*>(glfwGetWindowUserPointer(window));
            if (controller) controller->mouseButtonCallback(window, button, action, mods);
        });
        
        glfwSetCursorPosCallback(nativeWindow, [](GLFWwindow* window, double xpos, double ypos) {
            auto controller = static_cast<OrbitCameraController*>(glfwGetWindowUserPointer(window));
            if (controller) controller->cursorPosCallback(window, xpos, ypos);
        });
        
        glfwSetScrollCallback(nativeWindow, [](GLFWwindow* window, double xoffset, double yoffset) {
            auto controller = static_cast<OrbitCameraController*>(glfwGetWindowUserPointer(window));
            if (controller) controller->scrollCallback(window, xoffset, yoffset);
        });
    }
    
    void update(float deltaTime) override {
        // Process keyboard input for panning
        processKeyboardInput(deltaTime);
        
        // Apply changes
        updateCameraPosition();
    }
    
    void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {
        if (button == GLFW_MOUSE_BUTTON_LEFT) {
            if (action == GLFW_PRESS) {
                m_isDragging = true;
                glfwGetCursorPos(window, &m_lastMouseX, &m_lastMouseY);
            } else if (action == GLFW_RELEASE) {
                m_isDragging = false;
            }
        }
    }
    
    void cursorPosCallback(GLFWwindow* window, double xpos, double ypos) {
        if (m_isDragging) {
            // Calculate mouse movement
            float xOffset = static_cast<float>(xpos - m_lastMouseX);
            float yOffset = static_cast<float>(ypos - m_lastMouseY);
            
            // Update angles
            m_horizontalAngle -= xOffset * m_rotationSpeed * 0.1f;
            m_verticalAngle -= yOffset * m_rotationSpeed * 0.1f;
            
            // Clamp vertical angle to avoid flipping
            m_verticalAngle = std::max(-89.0f, std::min(89.0f, m_verticalAngle));
            
            // Store current mouse position
            m_lastMouseX = xpos;
            m_lastMouseY = ypos;
            
            // Update camera position
            updateCameraPosition();
        }
    }
    
    void scrollCallback(GLFWwindow* window, double xoffset, double yoffset) {
        // Zoom in/out with scroll wheel
        m_distance -= static_cast<float>(yoffset) * m_zoomSpeed;
        m_distance = std::max(1.0f, std::min(20.0f, m_distance));
        
        // Update camera position
        updateCameraPosition();
    }
    
    void processKeyboardInput(float deltaTime) {
        GLFWwindow* window = m_window->getNativeWindow();
        
        // WASD to pan the camera focus point
        Math::Vector3 panDelta(0, 0, 0);
        
        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
            panDelta.z -= m_panSpeed * deltaTime;
        }
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
            panDelta.z += m_panSpeed * deltaTime;
        }
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
            panDelta.x -= m_panSpeed * deltaTime;
        }
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
            panDelta.x += m_panSpeed * deltaTime;
        }
        
        // Apply panning relative to camera orientation
        float horizontalRad = m_horizontalAngle * 3.14159f / 180.0f;
        
        Math::Vector3 forward(
            sin(horizontalRad), 
            0,
            cos(horizontalRad)
        );
        
        Math::Vector3 right(
            cos(horizontalRad),
            0,
            -sin(horizontalRad)
        );
        
        m_focusPoint = m_focusPoint + forward * panDelta.z + right * panDelta.x;
    }
    
    void updateCameraPosition() {
        auto& transform = getOwner()->getComponent<TransformComponent>();
        auto& camera = getOwner()->getComponent<CameraComponent>();
        
        // Convert angles to radians
        float horizontalRad = m_horizontalAngle * 3.14159f / 180.0f;
        float verticalRad = m_verticalAngle * 3.14159f / 180.0f;
        
        // Calculate new camera position
        Math::Vector3 position;
        position.x = m_focusPoint.x + m_distance * cos(verticalRad) * sin(horizontalRad);
        position.y = m_focusPoint.y + m_distance * sin(verticalRad);
        position.z = m_focusPoint.z + m_distance * cos(verticalRad) * cos(horizontalRad);
        
        // Update transform
        transform.setPosition(position);
        
        // Ensure camera is looking at focus point
        camera.m_viewDirty = true;
        camera.m_target = m_focusPoint;
    }
};

} // namespace ECS
} // namespace Engine
