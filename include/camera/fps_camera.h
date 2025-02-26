#pragma once

#include "camera/camera.h"

namespace engine {
namespace rendering {

class FPSCamera : public Camera {
public:
    FPSCamera();
    virtual ~FPSCamera() = default;
    
    // Override base class methods
    virtual void update(float deltaTime) override;
    
    // FPS specific methods
    void processKeyboard(int direction, float deltaTime);
    void processMouseMovement(float xOffset, float yOffset);
    
    // Camera controls
    void setMovementSpeed(float speed) { m_movementSpeed = speed; }
    void setMouseSensitivity(float sensitivity) { m_mouseSensitivity = sensitivity; }
    
    // Camera options
    float m_yaw;
    float m_pitch;
    float m_movementSpeed;
    float m_mouseSensitivity;
    
    // Movement directions
    static const int FORWARD = 0;
    static const int BACKWARD = 1;
    static const int LEFT = 2;
    static const int RIGHT = 3;
    
    // Recalculate camera vectors from angles
    void updateCameraVectors();
};

} // namespace rendering
} // namespace engine
