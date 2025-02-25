#pragma once

#include <string>
#include <unordered_map>
#include <glm/glm.hpp>

namespace engine {
namespace rendering {

class Shader {
public:
    Shader();
    ~Shader();
    
    // Delete copy constructor and assignment operator
    Shader(const Shader&) = delete;
    Shader& operator=(const Shader&) = delete;
    
    // Load shaders from files
    bool loadFromFiles(const std::string& vertexPath, const std::string& fragmentPath);
    
    // Load shaders from strings
    bool loadFromSource(const std::string& vertexSource, const std::string& fragmentSource);
    
    // Use this shader program
    void use();
    
    // Uniform setters
    void setFloat(const std::string& name, float value);
    void setInt(const std::string& name, int value);
    void setBool(const std::string& name, bool value);
    void setVec2(const std::string& name, const glm::vec2& value);
    void setVec3(const std::string& name, const glm::vec3& value);
    void setVec4(const std::string& name, const glm::vec4& value);
    void setMat2(const std::string& name, const glm::mat2& value);
    void setMat3(const std::string& name, const glm::mat3& value);
    void setMat4(const std::string& name, const glm::mat4& value);
    
    // Get the program ID
    unsigned int getID() const { return m_programID; }
    
private:
    unsigned int m_programID;
    std::unordered_map<std::string, int> m_uniformLocationCache;
    
    // Helper methods
    bool compileShader(unsigned int& shader, const std::string& source, unsigned int type);
    bool linkProgram(unsigned int vertexShader, unsigned int fragmentShader);
    int getUniformLocation(const std::string& name);
    
    // Logging
    void logShaderError(unsigned int shader);
    void logProgramError(unsigned int program);
};

} // namespace rendering
} // namespace engine
