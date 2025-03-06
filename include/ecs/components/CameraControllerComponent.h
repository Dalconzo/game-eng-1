#pragma once
#include "../Component.h"
#include "../math/Vector.h"
#include "rendering/window.h"

namespace Engine {
namespace ECS {

class CameraControllerComponent : public Component {
private:
    float m_moveSpeed = 5.0f; // Units per second
    Math::Vector3 m_moveDirection;

public:
    CameraControllerComponent() = default;
    
    void setMoveSpeed(float speed) { m_moveSpeed = speed; }
    float getMoveSpeed() const { return m_moveSpeed; }
    
    void setMoveDirection(const Math::Vector3& direction) {
        m_moveDirection = direction;
    }
    
    // Handle WASD input and update movement direction
    void handleInput(engine::rendering::Window* window);
    
    // Apply movement to transform
    virtual void update(float deltaTime, TransformComponent& transform);
};

} // namespace ECS
} // namespace Engine
