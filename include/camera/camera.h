#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace engine {
namespace rendering {

// Camera types for specialized behavior
enum class CameraType {
    PERSPECTIVE,
    ORTHOGRAPHIC
};

class Camera {
public:
    Camera(CameraType type = CameraType::PERSPECTIVE);
    virtual ~Camera() = default;
    
    // Core camera functionality
    virtual void update(float deltaTime);
    
    // View matrix represents the camera's position and orientation
    virtual glm::mat4 getViewMatrix() const;
    
    // Projection matrix defines how 3D coordinates are projected to 2D
    virtual glm::mat4 getProjectionMatrix() const;
    
    // Position and orientation setters
    void setPosition(const glm::vec3& position);
    void setTarget(const glm::vec3& target);
    void setUp(const glm::vec3& up);
    
    // Projection parameters
    void setPerspective(float fov, float aspectRatio, float nearPlane, float farPlane);
    void setOrthographic(float left, float right, float bottom, float top, float nearPlane, float farPlane);
    
    // Getters
    glm::vec3 getPosition() const { return m_position; }
    glm::vec3 getTarget() const { return m_target; }
    glm::vec3 getUp() const { return m_up; }
    glm::vec3 getRight() const;
    glm::vec3 getForward() const;
    CameraType getType() const { return m_type; }
    
protected:
    // Transform properties
    glm::vec3 m_position;
    glm::vec3 m_target;
    glm::vec3 m_up;
    
    // Projection properties
    CameraType m_type;
    
    // Perspective properties
    float m_fov;
    float m_aspectRatio;
    float m_nearPlane;
    float m_farPlane;
    
    // Orthographic properties
    float m_left;
    float m_right;
    float m_bottom; 
    float m_top;
    
    // Cached matrices
    mutable bool m_viewDirty;
    mutable bool m_projectionDirty;
    mutable glm::mat4 m_viewMatrix;
    mutable glm::mat4 m_projectionMatrix;
};

} // namespace rendering
} // namespace engine
