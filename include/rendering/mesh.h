#pragma once

#include <vector>
#include <glm/glm.hpp>
#include "rendering/shader.h"

namespace engine {
namespace rendering {

class Mesh {
public:
    Mesh(const std::vector<float>& vertices, int positionComponents = 3, int colorComponents = 3);
    ~Mesh();
    
    // Delete copy constructor and assignment operator
    Mesh(const Mesh&) = delete;
    Mesh& operator=(const Mesh&) = delete;
    
    // Render the mesh with the given shader
    void render(Shader& shader);
    
    // Transform operations
    void setPosition(const glm::vec3& position) { m_position = position; }
    void setRotation(float angle, const glm::vec3& axis) { 
        m_rotationAngle = angle; 
        m_rotationAxis = axis; 
    }
    void setScale(const glm::vec3& scale) { m_scale = scale; }
    
    // Get transformation matrix
    glm::mat4 getModelMatrix() const;
    
private:
    unsigned int m_vao;
    unsigned int m_vbo;
    int m_vertexCount;
    int m_stride;
    
    // Transform properties
    glm::vec3 m_position = glm::vec3(0.0f);
    float m_rotationAngle = 0.0f;
    glm::vec3 m_rotationAxis = glm::vec3(0.0f, 1.0f, 0.0f);
    glm::vec3 m_scale = glm::vec3(1.0f);
};

} // namespace rendering
} // namespace engine
