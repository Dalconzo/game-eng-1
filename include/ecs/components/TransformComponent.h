#pragma once
#include "../Component.h"
#include "../math/Vector.h"
#include "../math/Quaternion.h"
#include "../math/Matrix4x4.h"

namespace Engine {
namespace ECS {

class TransformComponent : public Component {
public:
    Math::Vector3 position;
    Math::Quaternion rotation;
    Math::Vector3 scale;
    
    // Cached world matrix
    mutable bool worldMatrixDirty = true;
    mutable Math::Matrix4x4 worldMatrix;
    
    TransformComponent() 
        : position(Math::Vector3::zero()),
          rotation(Math::Quaternion::identity()),
          scale(Math::Vector3::one()),
          worldMatrixDirty(true) {
    }
    
    TransformComponent(const Math::Vector3& position, 
                       const Math::Quaternion& rotation = Math::Quaternion::identity(), 
                       const Math::Vector3& scale = Math::Vector3::one())
        : position(position),
          rotation(rotation),
          scale(scale),
          worldMatrixDirty(true) {
    }
    
    // Mark the transform as dirty when any property changes
    void setPosition(const Math::Vector3& newPosition) {
        position = newPosition;
        worldMatrixDirty = true;
    }
    
    void setRotation(const Math::Quaternion& newRotation) {
        rotation = newRotation;
        worldMatrixDirty = true;
    }
    
    void setScale(const Math::Vector3& newScale) {
        scale = newScale;
        worldMatrixDirty = true;
    }
    
    // Calculate and return the local transformation matrix
    Math::Matrix4x4 getLocalMatrix() const {
        return Math::Matrix4x4::createTRS(position, rotation, scale);
    }
    
    // Calculate and return the world transformation matrix
    Math::Matrix4x4 getWorldMatrix() const {
        if (worldMatrixDirty) {
            worldMatrix = getLocalMatrix();
            
            // If this entity has a parent with a transform component, multiply by parent's world matrix
            Entity* owner = getOwner();
            if (owner && owner->getParent()) {
                if (owner->getParent()->hasComponent<TransformComponent>()) {
                    const TransformComponent& parentTransform = owner->getParent()->getComponent<TransformComponent>();
                    worldMatrix = parentTransform.getWorldMatrix() * worldMatrix;
                }
            }
            
            worldMatrixDirty = false;
        }
        
        return worldMatrix;
    }
    
    // Transformation utility methods
    void translate(const Math::Vector3& translation) {
        position = position + translation;
        worldMatrixDirty = true;
    }
    
    void rotate(const Math::Quaternion& rotation) {
        this->rotation = rotation * this->rotation;
        worldMatrixDirty = true;
    }
    
    // Look at a target position
    void lookAt(const Math::Vector3& target, const Math::Vector3& up = Math::Vector3::up()) {
        // Calculate forward direction
        Math::Vector3 forward = (target - position).normalized();
        
        // Calculate right vector
        Math::Vector3 right = Math::Vector3::up().cross(forward).normalized();
        
        // Calculate adjusted up vector
        Math::Vector3 adjustedUp = forward.cross(right);
        
        // Build rotation matrix
        Math::Matrix4x4 lookMatrix;
        lookMatrix(0, 0) = right.x;
        lookMatrix(0, 1) = right.y;
        lookMatrix(0, 2) = right.z;
        
        lookMatrix(1, 0) = adjustedUp.x;
        lookMatrix(1, 1) = adjustedUp.y;
        lookMatrix(1, 2) = adjustedUp.z;
        
        lookMatrix(2, 0) = forward.x;
        lookMatrix(2, 1) = forward.y;
        lookMatrix(2, 2) = forward.z;
        
        // Extract rotation as quaternion
        rotation = Math::Quaternion::fromMatrix(lookMatrix);
        worldMatrixDirty = true;
    }
    
    // Override functions from Component base class
    virtual void init() override {
        worldMatrixDirty = true;
    }
    
    virtual void update(float deltaTime) override {
        // If there are animation or dynamic updates to transform, they would go here
    }
};

} // namespace ECS
} // namespace Engine
