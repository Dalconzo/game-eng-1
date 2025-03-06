#include "ecs/components/CameraComponent.h"
#include "ecs/components/TransformComponent.h"
#include "ecs/math/Matrix4x4.h"
#include "ecs/Entity.h"
#include <GL/glew.h>
#include <iostream>

namespace Engine {
namespace ECS {

void CameraComponent::init() {
    m_projectionDirty = true;
    m_viewDirty = true;
}

void CameraComponent::update(float deltaTime) {
    // Mark view matrix as dirty if the transform has changed
    if (getOwner()->hasComponent<TransformComponent>()) {
        if (getOwner()->getComponent<TransformComponent>().worldMatrixDirty) {
            m_viewDirty = true;
        }
    }
}

const Math::Matrix4x4& CameraComponent::getProjectionMatrix() const {
    if (m_projectionDirty) {
        if (m_projectionType == ProjectionType::Perspective) {
            // Create perspective projection
            float tanHalfFovy = tan(m_fieldOfView * 0.5f * 3.14159f / 180.0f);
            
            m_projectionMatrix = Math::Matrix4x4();
            m_projectionMatrix(0, 0) = 1.0f / (m_aspectRatio * tanHalfFovy);
            m_projectionMatrix(1, 1) = 1.0f / tanHalfFovy;
            m_projectionMatrix(2, 2) = -(m_farPlane + m_nearPlane) / (m_farPlane - m_nearPlane);
            m_projectionMatrix(2, 3) = -1.0f;
            m_projectionMatrix(3, 2) = -(2.0f * m_farPlane * m_nearPlane) / (m_farPlane - m_nearPlane);
            m_projectionMatrix(3, 3) = 0.0f;
        } else {
            // Create orthographic projection
            float halfHeight = m_orthoSize * 0.5f;
            float halfWidth = halfHeight * m_aspectRatio;
            
            m_projectionMatrix = Math::Matrix4x4();
            m_projectionMatrix(0, 0) = 1.0f / halfWidth;
            m_projectionMatrix(1, 1) = 1.0f / halfHeight;
            m_projectionMatrix(2, 2) = -2.0f / (m_farPlane - m_nearPlane);
            m_projectionMatrix(3, 2) = -(m_farPlane + m_nearPlane) / (m_farPlane - m_nearPlane);
        }
        
        m_projectionDirty = false;
    }
    
    return m_projectionMatrix;
}
const Math::Matrix4x4& CameraComponent::getViewMatrix() const {
    std::cout << "View matrix position: " 
              << getOwner()->getComponent<TransformComponent>().position.x << ", "
              << getOwner()->getComponent<TransformComponent>().position.y << ", "
              << getOwner()->getComponent<TransformComponent>().position.z << std::endl;

    if (m_viewDirty) {
        std::cout << "Camera view matrix is dirty, recalculating" << std::endl;
        
        if (getOwner()->hasComponent<TransformComponent>()) {
            const auto& transform = getOwner()->getComponent<TransformComponent>();
            
            // Get camera position
            Math::Vector3 position = transform.getPosition();
            
            // Create view matrix looking at target
            Math::Vector3 up(0.0f, 1.0f, 0.0f);
            m_viewMatrix = Math::Matrix4x4::createLookAt(position, m_target, up);
            
            std::cout << "View matrix created with position: " << position.x << "," 
                      << position.y << "," << position.z << ", looking at target" << std::endl;
            
            m_viewDirty = false;
        }
    }
    
    return m_viewMatrix;
}Math::Matrix4x4 CameraComponent::getViewProjectionMatrix() const {
    return getProjectionMatrix() * getViewMatrix();
}

void CameraComponent::clear() const {
    // Clear the framebuffer with the camera's clear color
    // glClearColor(m_clearColor.x, m_clearColor.y, m_clearColor.z, 1.0f);
    glClearColor(0.2f, 0.4f, 0.8f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

} // namespace ECS
} // namespace Engine
