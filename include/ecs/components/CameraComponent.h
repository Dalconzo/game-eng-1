#pragma once
#include "../Component.h"
#include "../math/Matrix4x4.h"
#include "../math/Vector.h"

namespace Engine {
namespace ECS {

enum class ProjectionType {
    Perspective,
    Orthographic
};

class CameraComponent : public Component {
private:
    ProjectionType m_projectionType = ProjectionType::Perspective;
    
    // Perspective parameters
    float m_fieldOfView = 60.0f;
    float m_aspectRatio = 16.0f / 9.0f;
    
    // Orthographic parameters
    float m_orthoSize = 5.0f;
    
    // Common parameters
    float m_nearPlane = 0.1f;
    float m_farPlane = 1000.0f;
    
    // Cached matrices
    mutable bool m_projectionDirty = true;
    mutable bool m_viewDirty = true;
    mutable Math::Matrix4x4 m_projectionMatrix;
    mutable Math::Matrix4x4 m_viewMatrix;
    
    // Camera properties
    Math::Vector3 m_clearColor = {0.2f, 0.3f, 0.3f};
    bool m_isMain = false;

public:
    CameraComponent() = default;
    
    // Setters for camera properties
    void setPerspective(float fov, float aspectRatio, float nearPlane, float farPlane) {
        m_projectionType = ProjectionType::Perspective;
        m_fieldOfView = fov;
        m_aspectRatio = aspectRatio;
        m_nearPlane = nearPlane;
        m_farPlane = farPlane;
        m_projectionDirty = true;
    }
    
    void setOrthographic(float size, float aspectRatio, float nearPlane, float farPlane) {
        m_projectionType = ProjectionType::Orthographic;
        m_orthoSize = size;
        m_aspectRatio = aspectRatio;
        m_nearPlane = nearPlane;
        m_farPlane = farPlane;
        m_projectionDirty = true;
    }
    
    void setClearColor(const Math::Vector3& color) {
        m_clearColor = color;
    }
    
    void setMain(bool isMain) {
        m_isMain = isMain;
    }
    
    bool isMain() const {
        return m_isMain;
    }
    
    void setAspectRatio(float aspectRatio) {
        m_aspectRatio = aspectRatio;
        m_projectionDirty = true;
    }
    
    // Getters for matrices
    const Math::Matrix4x4& getProjectionMatrix() const;
    const Math::Matrix4x4& getViewMatrix() const;
    Math::Matrix4x4 getViewProjectionMatrix() const;
    
    // Camera functionality
    void clear() const;
    
    // Component interface
    virtual void init() override;
    virtual void update(float deltaTime) override;
};

} // namespace ECS
} // namespace Engine
