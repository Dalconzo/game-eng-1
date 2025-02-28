#include "ecs/components/CameraComponent.h"
#include "ecs/components/TransformComponent.h"
#include "ecs/Entity.h"
#include <GL/glew.h>

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
    if (m_viewDirty) {
        if (getOwner()->hasComponent<TransformComponent>()) {
            const auto& transform = getOwner()->getComponent<TransformComponent>();
            
            // Get the world matrix of the camera
            Math::Matrix4x4 worldMatrix = transform.getWorldMatrix();
            
            // The view matrix is the inverse of the camera's world matrix
            // For a simple implementation, we can use the fact that:
            // - The inverse of a rotation matrix is its transpose
            // - The inverse of a translation is the negative of the translation
            
            // Extract the rotation part (3x3 matrix)
            Math::Matrix4x4 rotationInverse;
            for (int i = 0; i < 3; i++) {
                for (int j = 0; j < 3; j++) {
                    rotationInverse(i, j) = worldMatrix(j, i); // Transpose
                }
            }
            
            // Extract the translation
            Math::Vector3 translation(worldMatrix(0, 3), worldMatrix(1, 3), worldMatrix(2, 3));
            
            // Apply inverse rotation to the translation
            Math::Vector3 translationInverse;
            translationInverse.x = -(rotationInverse(0, 0) * translation.x + 
                                      rotationInverse(0, 1) * translation.y + 
                                      rotationInverse(0, 2) * translation.z);
                                      
            translationInverse.y = -(rotationInverse(1, 0) * translation.x + 
                                      rotationInverse(1, 1) * translation.y + 
                                      rotationInverse(1, 2) * translation.z);
                                      
            translationInverse.z = -(rotationInverse(2, 0) * translation.x + 
                                      rotationInverse(2, 1) * translation.y + 
                                      rotationInverse(2, 2) * translation.z);
            
            // Construct the view matrix
            m_viewMatrix = rotationInverse;
            m_viewMatrix(0, 3) = translationInverse.x;
            m_viewMatrix(1, 3) = translationInverse.y;
            m_viewMatrix(2, 3) = translationInverse.z;
            
            m_viewDirty = false;
        }
    }
    
    return m_viewMatrix;
}

Math::Matrix4x4 CameraComponent::getViewProjectionMatrix() const {
    return getProjectionMatrix() * getViewMatrix();
}

void CameraComponent::clear() const {
    // Clear the framebuffer with the camera's clear color
    glClearColor(m_clearColor.x, m_clearColor.y, m_clearColor.z, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

} // namespace ECS
} // namespace Engine
