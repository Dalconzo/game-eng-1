#pragma once

#include "camera/camera.h"
#include "camera/fps_camera.h"
#include "camera/third_person_camera.h"
#include <GLFW/glfw3.h>
#include <memory>

namespace engine {
namespace rendering {

// This class handles input and connects it to camera operations
class CameraController {
public:
    CameraController(GLFWwindow* window);
    ~CameraController() = default;
    
    // Set the active camera
    void setCamera(std::shared_ptr<Camera> camera);
    
    // Process input for the current camera
    void processInput(float deltaTime);
    
    // Mouse movement callback - should be called from GLFW callback
    void processMouseMovement(float xOffset, float yOffset);
    
    // Mouse scroll callback - should be called from GLFW callback
    void processMouseScroll(float yOffset);
    
    // Get the active camera
    std::shared_ptr<Camera> getCamera() const { return m_camera; }
    
private:
    GLFWwindow* m_window;
    std::shared_ptr<Camera> m_camera;
    
    // Input state tracking
    bool m_firstMouse;
    float m_lastX, m_lastY;
    
    // Process input specifically for FPS camera
    void processFpsInput(std::shared_ptr<FPSCamera> camera, float deltaTime);
    
    // Process input specifically for third-person camera
    void processThirdPersonInput(std::shared_ptr<ThirdPersonCamera> camera, float deltaTime);
};

} // namespace rendering
} // namespace engine
