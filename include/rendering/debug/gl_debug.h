#pragma once
#include <GL/glew.h>
#include <string>
#include <iostream>

namespace engine {
namespace rendering {
namespace debug {

// Platform-independent callback definition
#ifdef _WIN32
#define DEBUG_CALLBACK APIENTRY
#else
#define DEBUG_CALLBACK
#endif

// Debug callback for OpenGL
void DEBUG_CALLBACK glDebugOutput(GLenum source, GLenum type, GLuint id, 
                           GLenum severity, GLsizei length,
                           const GLchar *message, const void *userParam);

// Error checking utility
void checkGLError(const char* operation);

// Shader validation
void validateShaderCompilation(GLuint shader);
void validateProgramLinking(GLuint program);

// Setup debug context (call this once during initialization)
void setupGLDebugContext();

} // namespace debug
} // namespace rendering
} // namespace engine
