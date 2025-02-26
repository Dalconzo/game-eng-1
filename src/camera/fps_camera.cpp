#include "camera/fps_camera.h"
#include <algorithm>
#include <glm/gtc/constants.hpp>

namespace engine {
namespace rendering {

FPSCamera::FPSCamera()
    : Camera(CameraType::PERSPECTIVE)
    , m_yaw(-90.0f)
    , m_pitch(0.0f)
    , m_movementSpeed(5.0f)
    , m_mouseSensitivity(0.1f)
{
    // Initialize with default position
    m_position = glm::vec3(0.0f, 0.0f, 3.0f);
    updateCameraVectors();
}

void FPSCamera::update(float deltaTime) {
    // FPS camera's update would handle any continuous movement
    // Most movement is handled directly in processKeyboard and processMouseMovement
}

void FPSCamera::processKeyboard(int direction, float deltaTime) {
    float velocity = m_movementSpeed * deltaTime;
    
    if (direction == FORWARD)
        m_position += getForward() * velocity;
    if (direction == BACKWARD)
        m_position -= getForward() * velocity;
    if (direction == LEFT)
        m_position -= getRight() * velocity;
    if (direction == RIGHT)
        m_position += getRight() * velocity;
    
    // Update target based on new position
    m_target = m_position + getForward();
    m_viewDirty = true;
}

void FPSCamera::processMouseMovement(float xOffset, float yOffset) {
    xOffset *= m_mouseSensitivity;
    yOffset *= m_mouseSensitivity;
    
    m_yaw += xOffset;
    m_pitch += yOffset;
    
    // Prevent flipping by constraining pitch
    m_pitch = std::clamp(m_pitch, -89.0f, 89.0f);
    
    // Update camera vectors
    updateCameraVectors();
}

void FPSCamera::updateCameraVectors() {
    // Calculate the new forward vector
    glm::vec3 front;
    front.x = cos(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
    front.y = sin(glm::radians(m_pitch));
    front.z = sin(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
    
    // Update target
    glm::vec3 forward = glm::normalize(front);
    m_target = m_position + forward;
    
    // Also re-calculate the Right and Up vector
    m_viewDirty = true;
}

} // namespace rendering
} // namespace engine
