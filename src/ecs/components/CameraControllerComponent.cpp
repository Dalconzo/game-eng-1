#include "ecs/components/CameraControllerComponent.h"
#include "ecs/components/TransformComponent.h"
#include "rendering/window.h"
#include <GLFW/glfw3.h>

namespace Engine {
namespace ECS {

void CameraControllerComponent::handleInput(engine::rendering::Window* window) {
    // Reset movement direction
    m_moveDirection = Math::Vector3(0, 0, 0);
    
    // Get window's native GLFW handle
    GLFWwindow* nativeWindow = window->getNativeWindow();
    
    // Check WASD keys
    if (glfwGetKey(nativeWindow, GLFW_KEY_W) == GLFW_PRESS) {
        m_moveDirection.z -= 1.0f; // Forward
    }
    if (glfwGetKey(nativeWindow, GLFW_KEY_S) == GLFW_PRESS) {
        m_moveDirection.z += 1.0f; // Backward
    }
    if (glfwGetKey(nativeWindow, GLFW_KEY_A) == GLFW_PRESS) {
        m_moveDirection.x -= 1.0f; // Left
    }
    if (glfwGetKey(nativeWindow, GLFW_KEY_D) == GLFW_PRESS) {
        m_moveDirection.x += 1.0f; // Right
    }
    
    // Normalize if moving in multiple directions
    if (m_moveDirection.magnitude() > 0.0f) {
        m_moveDirection = m_moveDirection.normalized();
    }
}

void CameraControllerComponent::update(float deltaTime, TransformComponent& transform) {
    // Get transform component
    if (getOwner()->hasComponent<Engine::ECS::TransformComponent>()) {
        auto& transform = getOwner()->getComponent<Engine::ECS::TransformComponent>();
        // Apply movement based on direction and speed
        Math::Vector3 movement = m_moveDirection * m_moveSpeed * deltaTime;
        Math::Vector3 newPosition = transform.getPosition() + movement;
        transform.setPosition(newPosition);
    }
}

} // namespace ECS
} // namespace Engine
