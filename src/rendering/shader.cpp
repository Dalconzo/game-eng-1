#include "rendering/shader.h"
#include <GL/glew.h>
#include <fstream>
#include <sstream>
#include <iostream>
#include <glm/gtc/type_ptr.hpp>
#include "rendering/debug/gl_debug.h"

namespace engine {
namespace rendering {

Shader::Shader() : m_programID(0) {
}

Shader::~Shader() {
    if (m_programID) {
        glDeleteProgram(m_programID);
    }
}

bool Shader::loadFromFiles(const std::string& vertexPath, const std::string& fragmentPath) {
    // Read vertex shader from file
    std::string vertexSource;
    std::ifstream vertexFile;
    vertexFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    
    try {
        vertexFile.open(vertexPath);
        std::stringstream vertexStream;
        vertexStream << vertexFile.rdbuf();
        vertexFile.close();
        vertexSource = vertexStream.str();
    } catch (std::ifstream::failure& e) {
        std::cerr << "ERROR::SHADER::VERTEX::FILE_NOT_SUCCESSFULLY_READ: " << e.what() << std::endl;
        return false;
    }
    
    // Read fragment shader from file
    std::string fragmentSource;
    std::ifstream fragmentFile;
    fragmentFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    
    try {
        fragmentFile.open(fragmentPath);
        std::stringstream fragmentStream;
        fragmentStream << fragmentFile.rdbuf();
        fragmentFile.close();
        fragmentSource = fragmentStream.str();
    } catch (std::ifstream::failure& e) {
        std::cerr << "ERROR::SHADER::FRAGMENT::FILE_NOT_SUCCESSFULLY_READ: " << e.what() << std::endl;
        return false;
    }
    
    return loadFromSource(vertexSource, fragmentSource);
}

bool Shader::loadFromSource(const std::string& vertexSource, const std::string& fragmentSource) {
    // Clean up previous program if any
    if (m_programID) {
        glDeleteProgram(m_programID);
        m_uniformLocationCache.clear();
    }
    
    unsigned int vertexShader = 0, fragmentShader = 0;
    bool success = true;
    
    // Compile shaders
    success = compileShader(vertexShader, vertexSource, GL_VERTEX_SHADER);
    if (!success) return false;
    
    success = compileShader(fragmentShader, fragmentSource, GL_FRAGMENT_SHADER);
    if (!success) {
        glDeleteShader(vertexShader);
        return false;
    }
    
    // Link shaders
    success = linkProgram(vertexShader, fragmentShader);
    
    // Shaders can be deleted after linking
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    
    return success;
}

bool Shader::compileShader(unsigned int& shader, const std::string& source, unsigned int type) {
    shader = glCreateShader(type);
    const char* sourceCode = source.c_str();
    glShaderSource(shader, 1, &sourceCode, NULL);
    glCompileShader(shader);

    engine::rendering::debug::validateShaderCompilation(shader);
    
    // Check for compilation errors
    int success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        logShaderError(shader);
        return false;
    }
    
    return true;
}

bool Shader::linkProgram(unsigned int vertexShader, unsigned int fragmentShader) {
    m_programID = glCreateProgram();
    glAttachShader(m_programID, vertexShader);
    glAttachShader(m_programID, fragmentShader);
    glLinkProgram(m_programID);

    engine::rendering::debug::validateProgramLinking(m_programID);
    
    // Check for linking errors
    int success;
    glGetProgramiv(m_programID, GL_LINK_STATUS, &success);
    if (!success) {
        logProgramError(m_programID);
        return false;
    }
    
    return true;
}

void Shader::use() {
    glUseProgram(m_programID);
    std::cout << "Binding shader program ID: " << m_programID << std::endl;
}

int Shader::getUniformLocation(const std::string& name) {
    // Check if we've already retrieved this location
    auto it = m_uniformLocationCache.find(name);
    if (it != m_uniformLocationCache.end()) {
        return it->second;
    }
    
    // Get the location and cache it
    int location = glGetUniformLocation(m_programID, name.c_str());
    m_uniformLocationCache[name] = location;
    
    if (location == -1) {
        std::cerr << "WARNING: Uniform '" << name << "' doesn't exist or isn't used" << std::endl;
    }
    
    return location;
}

void Shader::setFloat(const std::string& name, float value) {
    glUniform1f(getUniformLocation(name), value);
}

void Shader::setInt(const std::string& name, int value) {
    glUniform1i(getUniformLocation(name), value);
}

void Shader::setBool(const std::string& name, bool value) {
    glUniform1i(getUniformLocation(name), static_cast<int>(value));
}

void Shader::setVec2(const std::string& name, const glm::vec2& value) {
    glUniform2fv(getUniformLocation(name), 1, glm::value_ptr(value));
}

void Shader::setVec3(const std::string& name, const glm::vec3& value) {
    glUniform3fv(getUniformLocation(name), 1, glm::value_ptr(value));
}

void Shader::setVec4(const std::string& name, const glm::vec4& value) {
    glUniform4fv(getUniformLocation(name), 1, glm::value_ptr(value));
}

void Shader::setMat2(const std::string& name, const glm::mat2& value) {
    glUniformMatrix2fv(getUniformLocation(name), 1, GL_FALSE, glm::value_ptr(value));
}

void Shader::setMat3(const std::string& name, const glm::mat3& value) {
    glUniformMatrix3fv(getUniformLocation(name), 1, GL_FALSE, glm::value_ptr(value));
}

void Shader::setMat4(const std::string& name, const glm::mat4& value) {
    unsigned int location = glGetUniformLocation(m_programID, name.c_str());
    glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(value));
    
    std::cout << "Set uniform '" << name << "' at location " << location << std::endl;
    
    // Print full matrix representation
    std::cout << "  Matrix values: [" << std::endl;
    for (int row = 0; row < 4; row++) {
        std::cout << "    ";
        for (int col = 0; col < 4; col++) {
            std::cout << value[col][row] << " ";
        }
        std::cout << std::endl;
    }
    std::cout << "  ]" << std::endl;
}

void Shader::logShaderError(unsigned int shader) {
    int logLength = 0;
    glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logLength);
    
    std::vector<char> log(logLength);
    glGetShaderInfoLog(shader, logLength, NULL, log.data());
    
    std::cerr << "ERROR::SHADER::COMPILATION_FAILED\n" << log.data() << std::endl;
}

void Shader::logProgramError(unsigned int program) {
    int logLength = 0;
    glGetProgramiv(program, GL_INFO_LOG_LENGTH, &logLength);
    
    std::vector<char> log(logLength);
    glGetProgramInfoLog(program, logLength, NULL, log.data());
    
    std::cerr << "ERROR::PROGRAM::LINKING_FAILED\n" << log.data() << std::endl;
}

} // namespace rendering
} // namespace engine
