#pragma once

#include "rendering/shader.h"
#include <vector>
#include <glm/glm.hpp>
#include <string>
#include <memory>

namespace engine {
namespace rendering {

// Enhanced Vertex structure to hold all per-vertex attributes
struct Vertex {
    glm::vec3 position = glm::vec3(0.0f);
    glm::vec3 normal = glm::vec3(0.0f, 1.0f, 0.0f);
    glm::vec2 texCoord = glm::vec2(0.0f);
    glm::vec3 tangent = glm::vec3(1.0f, 0.0f, 0.0f);
    glm::vec3 bitangent = glm::vec3(0.0f, 0.0f, 1.0f);
};

class Mesh {
public:
    Mesh();
    ~Mesh();
    
    // Delete copy constructor and assignment operator
    Mesh(const Mesh&) = delete;
    Mesh& operator=(const Mesh&) = delete;
    
    // Initialize mesh with vertex data
    void setVertices(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices);
    
    // Compute tangent space vectors for normal mapping
    void computeTangentBasis();
    
    // Render the mesh
    void render(Shader& shader);
    
    // Getters
    const std::vector<Vertex>& getVertices() const { return m_vertices; }
    const std::vector<unsigned int>& getIndices() const { return m_indices; }
    
    // Transform operations
    void setPosition(const glm::vec3& position) { m_position = position; }
    void setRotation(float angle, const glm::vec3& axis) { 
        m_rotationAngle = angle; 
        m_rotationAxis = axis; 
    }
    void setScale(const glm::vec3& scale) { m_scale = scale; }
    
    // Get transformation matrix
    glm::mat4 getModelMatrix() const;
    
    // Mesh identification
    void setName(const std::string& name) { m_name = name; }
    const std::string& getName() const { return m_name; }
    
private:
    // OpenGL objects
    unsigned int m_VAO;
    unsigned int m_VBO;
    unsigned int m_EBO;
    
    // Mesh data
    std::vector<Vertex> m_vertices;
    std::vector<unsigned int> m_indices;
    std::string m_name;
    
    // Set up mesh buffers
    void setupMesh();
    
    // Transform properties
    glm::vec3 m_position = glm::vec3(0.0f);
    float m_rotationAngle = 0.0f;
    glm::vec3 m_rotationAxis = glm::vec3(0.0f, 1.0f, 0.0f);
    glm::vec3 m_scale = glm::vec3(1.0f);
};

} // namespace rendering
} // namespace engine
