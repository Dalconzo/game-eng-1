#pragma once

#include "camera/camera.h"

namespace engine {
namespace rendering {

class ThirdPersonCamera : public Camera {
public:
    ThirdPersonCamera();
    virtual ~ThirdPersonCamera() = default;
    
    // Override base class methods
    virtual void update(float deltaTime) override;
    
    // Set the position to follow
    void setTargetPosition(const glm::vec3& targetPosition);
    
    // Orbit controls
    void orbit(float horizontalAngle, float verticalAngle);
    
    // Zoom controls
    void zoom(float amount);
    
    // Camera properties
    void setDistance(float distance);
    void setOrbitSpeed(float speed) { m_orbitSpeed = speed; }
    void setZoomSpeed(float speed) { m_zoomSpeed = speed; }
    
private:
    // Camera properties
    glm::vec3 m_targetPosition;
    float m_distance;
    float m_horizontalAngle;
    float m_verticalAngle;
    
    // Control speeds
    float m_orbitSpeed;
    float m_zoomSpeed;
    
    // Minimum/maximum values
    float m_minDistance;
    float m_maxDistance;
    float m_minVerticalAngle;
    float m_maxVerticalAngle;
    
    // Recalculate camera position based on angles and distance
    void updateCameraPosition();
};

} // namespace rendering
} // namespace engine
