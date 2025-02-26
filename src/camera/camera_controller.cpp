#include "camera/camera_controller.h"
#include <iostream>

namespace engine {
namespace rendering {

CameraController::CameraController(GLFWwindow* window)
    : m_window(window)
    , m_camera(nullptr)
    , m_firstMouse(true)
    , m_lastX(0.0f)
    , m_lastY(0.0f)
{
    // Get initial window size for mouse positioning
    int width, height;
    glfwGetWindowSize(window, &width, &height);
    m_lastX = width / 2.0f;
    m_lastY = height / 2.0f;
}

void CameraController::setCamera(std::shared_ptr<Camera> camera) {
    m_camera = camera;
    m_firstMouse = true; // Reset mouse tracking when camera changes
}

void CameraController::processInput(float deltaTime) {
    if (!m_camera || !m_window) {
        return;
    }
    
    // Check camera type and process appropriate input
    auto fpsCamera = std::dynamic_pointer_cast<FPSCamera>(m_camera);
    if (fpsCamera) {
        processFpsInput(fpsCamera, deltaTime);
        return;
    }
    
    auto thirdPersonCamera = std::dynamic_pointer_cast<ThirdPersonCamera>(m_camera);
    if (thirdPersonCamera) {
        processThirdPersonInput(thirdPersonCamera, deltaTime);
        return;
    }
}

void CameraController::processFpsInput(std::shared_ptr<FPSCamera> camera, float deltaTime) {
    // Process keyboard input for movement
    if (glfwGetKey(m_window, GLFW_KEY_W) == GLFW_PRESS)
        camera->processKeyboard(FPSCamera::FORWARD, deltaTime);
    if (glfwGetKey(m_window, GLFW_KEY_S) == GLFW_PRESS)
        camera->processKeyboard(FPSCamera::BACKWARD, deltaTime);
    if (glfwGetKey(m_window, GLFW_KEY_A) == GLFW_PRESS)
        camera->processKeyboard(FPSCamera::LEFT, deltaTime);
    if (glfwGetKey(m_window, GLFW_KEY_D) == GLFW_PRESS)
        camera->processKeyboard(FPSCamera::RIGHT, deltaTime);
}

void CameraController::processThirdPersonInput(std::shared_ptr<ThirdPersonCamera> camera, float deltaTime) {
    // For third-person camera, most control happens via mouse in processMouseMovement
    // Here we might process keys for moving the target
    
    // Simple example: Move target with arrow keys
    glm::vec3 targetPos = camera->getTarget();
    float speed = 2.0f * deltaTime;
    
    if (glfwGetKey(m_window, GLFW_KEY_UP) == GLFW_PRESS)
        targetPos.z -= speed;
    if (glfwGetKey(m_window, GLFW_KEY_DOWN) == GLFW_PRESS)
        targetPos.z += speed;
    if (glfwGetKey(m_window, GLFW_KEY_LEFT) == GLFW_PRESS)
        targetPos.x -= speed;
    if (glfwGetKey(m_window, GLFW_KEY_RIGHT) == GLFW_PRESS)
        targetPos.x += speed;
    
    camera->setTargetPosition(targetPos);
}

void CameraController::processMouseMovement(float xPos, float yPos) {
    if (!m_camera) {
        return;
    }
    
    if (m_firstMouse) {
        m_lastX = xPos;
        m_lastY = yPos;
        m_firstMouse = false;
    }
    
    float xOffset = xPos - m_lastX;
    float yOffset = m_lastY - yPos; // Reversed since y-coordinates range from bottom to top
    
    m_lastX = xPos;
    m_lastY = yPos;
    
    // Process based on camera type
    auto fpsCamera = std::dynamic_pointer_cast<FPSCamera>(m_camera);
    if (fpsCamera) {
        fpsCamera->processMouseMovement(xOffset, yOffset);
        return;
    }
    
    auto thirdPersonCamera = std::dynamic_pointer_cast<ThirdPersonCamera>(m_camera);
    if (thirdPersonCamera && glfwGetMouseButton(m_window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
        thirdPersonCamera->orbit(xOffset, yOffset);
        return;
    }
}

void CameraController::processMouseScroll(float yOffset) {
    if (!m_camera) {
        return;
    }
    
    // For third-person camera, scroll controls zoom
    auto thirdPersonCamera = std::dynamic_pointer_cast<ThirdPersonCamera>(m_camera);
    if (thirdPersonCamera) {
        thirdPersonCamera->zoom(yOffset);
    }
    
    // For FPS camera, could adjust speed or FOV
    auto fpsCamera = std::dynamic_pointer_cast<FPSCamera>(m_camera);
    if (fpsCamera) {
        // Example: adjust FOV (simple zoom effect)
        // Would need to add an appropriate method to FPSCamera
    }
}

} // namespace rendering
} // namespace engine
