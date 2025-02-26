#include "camera/camera.h"

namespace engine {
namespace rendering {

Camera::Camera(CameraType type)
    : m_position(0.0f, 0.0f, 5.0f)
    , m_target(0.0f, 0.0f, 0.0f)
    , m_up(0.0f, 1.0f, 0.0f)
    , m_type(type)
    , m_fov(45.0f)
    , m_aspectRatio(16.0f / 9.0f)
    , m_nearPlane(0.1f)
    , m_farPlane(1000.0f)
    , m_left(-10.0f)
    , m_right(10.0f)
    , m_bottom(-10.0f)
    , m_top(10.0f)
    , m_viewDirty(true)
    , m_projectionDirty(true)
{
}

void Camera::update(float deltaTime) {
    // Base camera has no automatic movement
    // Derived classes would implement behavior here
}

glm::mat4 Camera::getViewMatrix() const {
    if (m_viewDirty) {
        m_viewMatrix = glm::lookAt(m_position, m_target, m_up);
        m_viewDirty = false;
    }
    return m_viewMatrix;
}

glm::mat4 Camera::getProjectionMatrix() const {
    if (m_projectionDirty) {
        if (m_type == CameraType::PERSPECTIVE) {
            m_projectionMatrix = glm::perspective(glm::radians(m_fov), m_aspectRatio, m_nearPlane, m_farPlane);
        } else {
            m_projectionMatrix = glm::ortho(m_left, m_right, m_bottom, m_top, m_nearPlane, m_farPlane);
        }
        m_projectionDirty = false;
    }
    return m_projectionMatrix;
}

void Camera::setPosition(const glm::vec3& position) {
    m_position = position;
    m_viewDirty = true;
}

void Camera::setTarget(const glm::vec3& target) {
    m_target = target;
    m_viewDirty = true;
}

void Camera::setUp(const glm::vec3& up) {
    m_up = glm::normalize(up);
    m_viewDirty = true;
}

void Camera::setPerspective(float fov, float aspectRatio, float nearPlane, float farPlane) {
    m_type = CameraType::PERSPECTIVE;
    m_fov = fov;
    m_aspectRatio = aspectRatio;
    m_nearPlane = nearPlane;
    m_farPlane = farPlane;
    m_projectionDirty = true;
}

void Camera::setOrthographic(float left, float right, float bottom, float top, float nearPlane, float farPlane) {
    m_type = CameraType::ORTHOGRAPHIC;
    m_left = left;
    m_right = right;
    m_bottom = bottom;
    m_top = top;
    m_nearPlane = nearPlane;
    m_farPlane = farPlane;
    m_projectionDirty = true;
}

glm::vec3 Camera::getRight() const {
    return glm::normalize(glm::cross(m_target - m_position, m_up));
}

glm::vec3 Camera::getForward() const {
    return glm::normalize(m_target - m_position);
}

} // namespace rendering
} // namespace engine
