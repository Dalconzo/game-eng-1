#include "rendering/window.h"
#include <GLFW/glfw3.h>
#include <iostream>
#include <stdexcept>

namespace engine {
namespace rendering {

// Store Window instance pointer for callbacks
static std::unordered_map<GLFWwindow*, Window*> windowMap;

Window::Window(int width, int height, const std::string& title)
    : m_width(width), m_height(height), m_title(title), m_window(nullptr) {
    init();
}

Window::~Window() {
    if (m_window) {
        windowMap.erase(m_window);
        glfwDestroyWindow(m_window);
    }
    
    // Check if this is the last window - if so, terminate GLFW
    if (windowMap.empty()) {
        glfwTerminate();
    }
}

void Window::init() {
    // Initialize GLFW if it hasn't been already
    if (!glfwInit()) {
        throw std::runtime_error("Failed to initialize GLFW");
    }
    
    // Set OpenGL version and profile
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    
    #ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    #endif
    
    // Create the window
    m_window = glfwCreateWindow(m_width, m_height, m_title.c_str(), nullptr, nullptr);
    if (!m_window) {
        glfwTerminate();
        throw std::runtime_error("Failed to create GLFW window");
    }
    
    // Store this instance for callbacks
    windowMap[m_window] = this;
    
    // Make OpenGL context current
    glfwMakeContextCurrent(m_window);
    
    // Set callbacks
    glfwSetFramebufferSizeCallback(m_window, framebufferSizeCallback);
    
    // Enable VSync
    glfwSwapInterval(1);
}

void Window::swapBuffers() {
    glfwSwapBuffers(m_window);
}

void Window::pollEvents() {
    glfwPollEvents();
}

bool Window::shouldClose() const {
    return glfwWindowShouldClose(m_window);
}

void Window::close() {
    glfwSetWindowShouldClose(m_window, GLFW_TRUE);
}

void Window::setResizeCallback(ResizeCallback callback) {
    m_resizeCallback = std::move(callback);
}

void Window::framebufferSizeCallback(GLFWwindow* window, int width, int height) {
    // Find the Window instance for this GLFW window
    auto it = windowMap.find(window);
    if (it != windowMap.end()) {
        Window* windowInstance = it->second;
        windowInstance->m_width = width;
        windowInstance->m_height = height;
        
        // Call user-supplied callback if set
        if (windowInstance->m_resizeCallback) {
            windowInstance->m_resizeCallback(width, height);
        }
    }
    
    // Update viewport
    glViewport(0, 0, width, height);
}

} // namespace rendering
} // namespace engine
