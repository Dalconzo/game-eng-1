#pragma once

#include <string>
#include <functional>

// Forward declaration to avoid including GLFW header in our interface
struct GLFWwindow;

namespace engine {
namespace rendering {

class Window {
public:
    Window(int width, int height, const std::string& title);
    ~Window();

    // Delete copy constructor and assignment operator
    Window(const Window&) = delete;
    Window& operator=(const Window&) = delete;

    // Core window functions
    void swapBuffers();
    void pollEvents();
    bool shouldClose() const;
    void close();
    
    // Getters
    int getWidth() const { return m_width; }
    int getHeight() const { return m_height; }
    float getAspectRatio() const { return static_cast<float>(m_width) / m_height; }
    GLFWwindow* getNativeWindow() const { return m_window; }
    
    // Callback setters
    using ResizeCallback = std::function<void(int, int)>;
    void setResizeCallback(ResizeCallback callback);
    
private:
    GLFWwindow* m_window;
    int m_width;
    int m_height;
    std::string m_title;
    ResizeCallback m_resizeCallback;
    
    // Static callback for GLFW
    static void framebufferSizeCallback(GLFWwindow* window, int width, int height);
    
    // Initialize GLFW and create window
    void init();
};

} // namespace rendering
} // namespace engine
