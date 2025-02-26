
#include "camera/third_person_camera.h"
#include <algorithm>
#include <glm/gtc/constants.hpp>

namespace engine {
namespace rendering {

ThirdPersonCamera::ThirdPersonCamera()
    : Camera(CameraType::PERSPECTIVE)
    , m_targetPosition(0.0f, 0.0f, 0.0f)
    , m_distance(5.0f)
    , m_horizontalAngle(0.0f)
    , m_verticalAngle(30.0f)
    , m_orbitSpeed(0.25f)
    , m_zoomSpeed(0.5f)
    , m_minDistance(1.0f)
    , m_maxDistance(20.0f)
    , m_minVerticalAngle(-85.0f)
    , m_maxVerticalAngle(85.0f)
{
    // Initialize camera position based on target and angles
    updateCameraPosition();
}

void ThirdPersonCamera::update(float deltaTime) {
    // Third-person camera could implement smooth transitions here
    // For now, it just uses the direct position from updateCameraPosition
}

void ThirdPersonCamera::setTargetPosition(const glm::vec3& targetPosition) {
    m_targetPosition = targetPosition;
    m_target = m_targetPosition; // Look at the target position
    updateCameraPosition();
}

void ThirdPersonCamera::orbit(float horizontalAngle, float verticalAngle) {
    // Apply orbit speed
    m_horizontalAngle += horizontalAngle * m_orbitSpeed;
    m_verticalAngle += verticalAngle * m_orbitSpeed;
    
    // Wrap horizontal angle to [0, 360] degrees
    while (m_horizontalAngle >= 360.0f) {
        m_horizontalAngle -= 360.0f;
    }
    while (m_horizontalAngle < 0.0f) {
        m_horizontalAngle += 360.0f;
    }
    
    // Clamp vertical angle to avoid flipping
    m_verticalAngle = std::clamp(m_verticalAngle, m_minVerticalAngle, m_maxVerticalAngle);
    
    // Update camera position
    updateCameraPosition();
}

void ThirdPersonCamera::zoom(float amount) {
    // Apply zoom speed
    m_distance -= amount * m_zoomSpeed;
    
    // Clamp distance to avoid going inside objects or too far away
    m_distance = std::clamp(m_distance, m_minDistance, m_maxDistance);
    
    // Update camera position
    updateCameraPosition();
}

void ThirdPersonCamera::setDistance(float distance) {
    m_distance = std::clamp(distance, m_minDistance, m_maxDistance);
    updateCameraPosition();
}

void ThirdPersonCamera::updateCameraPosition() {
    // Convert angles to radians
    float horizontalRad = glm::radians(m_horizontalAngle);
    float verticalRad = glm::radians(m_verticalAngle);
    
    // Calculate position using spherical coordinates
    float x = m_distance * cos(verticalRad) * cos(horizontalRad);
    float y = m_distance * sin(verticalRad);
    float z = m_distance * cos(verticalRad) * sin(horizontalRad);
    
    // Position the camera relative to the target
    m_position = m_targetPosition + glm::vec3(x, y, z);
    
    // Ensure we're looking at the target
    m_target = m_targetPosition;
    
    // Mark view matrix as dirty to ensure it gets recalculated
    m_viewDirty = true;
}

} // namespace rendering
} // namespace engine
