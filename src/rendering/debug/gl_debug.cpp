#include "rendering/debug/gl_debug.h"
#include <vector>

namespace engine {
namespace rendering {
namespace debug {

void DEBUG_CALLBACK glDebugOutput(GLenum source, GLenum type, GLuint id, 
                           GLenum severity, GLsizei length,
                           const GLchar *message, const void *userParam) {
    // Print the debug message
    std::cout << "OpenGL Debug: " << message << std::endl;
}

void checkGLError(const char* operation) {
    GLenum error;
    while ((error = glGetError()) != GL_NO_ERROR) {
        std::string errorStr;
        switch (error) {
            case GL_INVALID_ENUM: errorStr = "GL_INVALID_ENUM"; break;
            case GL_INVALID_VALUE: errorStr = "GL_INVALID_VALUE"; break;
            case GL_INVALID_OPERATION: errorStr = "GL_INVALID_OPERATION"; break;
            case GL_OUT_OF_MEMORY: errorStr = "GL_OUT_OF_MEMORY"; break;
            default: errorStr = "Unknown"; break;
        }
        std::cout << "GL Error after " << operation << ": " << errorStr << std::endl;
    }
}

void validateShaderCompilation(GLuint shader) {
    GLint compiled = 0;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
    if (compiled == GL_FALSE) {
        GLint maxLength = 0;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);
        std::vector<GLchar> errorLog(maxLength);
        glGetShaderInfoLog(shader, maxLength, &maxLength, &errorLog[0]);
        std::cout << "Shader compilation error: " << &errorLog[0] << std::endl;
    }
}

void validateProgramLinking(GLuint program) {
    GLint linked = 0;
    glGetProgramiv(program, GL_LINK_STATUS, &linked);
    if (linked == GL_FALSE) {
        GLint maxLength = 0;
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);
        std::vector<GLchar> errorLog(maxLength);
        glGetProgramInfoLog(program, maxLength, &maxLength, &errorLog[0]);
        std::cout << "Program linking error: " << &errorLog[0] << std::endl;
    }
}

void setupGLDebugContext() {
    if (GLEW_ARB_debug_output) {
        glEnable(GL_DEBUG_OUTPUT);
        glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
        glDebugMessageCallback(glDebugOutput, nullptr);
        std::cout << "OpenGL debug context enabled" << std::endl;
    } else {
        std::cout << "OpenGL debug output not supported" << std::endl;
    }
}

} // namespace debug
} // namespace rendering
} // namespace engine
