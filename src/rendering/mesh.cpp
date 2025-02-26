#include "rendering/mesh.h"
#include <GL/glew.h>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

namespace engine {
namespace rendering {

Mesh::Mesh()
    : m_VAO(0)
    , m_VBO(0)
    , m_EBO(0)
{
}

Mesh::~Mesh() {
    // Clean up OpenGL resources
    if (m_VAO != 0) {
        glDeleteVertexArrays(1, &m_VAO);
    }
    if (m_VBO != 0) {
        glDeleteBuffers(1, &m_VBO);
    }
    if (m_EBO != 0) {
        glDeleteBuffers(1, &m_EBO);
    }
}

void Mesh::setVertices(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices) {
    m_vertices = vertices;
    m_indices = indices;
    
    // Set up the mesh with the new data
    setupMesh();
    
    // Compute tangent basis if we have texture coordinates
    if (!vertices.empty() && vertices[0].texCoord != glm::vec2(0.0f)) {
        computeTangentBasis();
    }
}

void Mesh::computeTangentBasis() {
    // Skip if we don't have enough indices for triangles
    if (m_indices.size() < 3) {
        return;
    }
    
    // Process each triangle
    for (size_t i = 0; i < m_indices.size(); i += 3) {
        Vertex& v0 = m_vertices[m_indices[i]];
        Vertex& v1 = m_vertices[m_indices[i + 1]];
        Vertex& v2 = m_vertices[m_indices[i + 2]];
        
        // Calculate edges of the triangle
        glm::vec3 edge1 = v1.position - v0.position;
        glm::vec3 edge2 = v2.position - v0.position;
        
        // Calculate texture coordinate differences
        glm::vec2 deltaUV1 = v1.texCoord - v0.texCoord;
        glm::vec2 deltaUV2 = v2.texCoord - v0.texCoord;
        
        // Calculate tangent and bitangent
        float f = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);
        
        glm::vec3 tangent;
        tangent.x = f * (deltaUV2.y * edge1.x - deltaUV1.y * edge2.x);
        tangent.y = f * (deltaUV2.y * edge1.y - deltaUV1.y * edge2.y);
        tangent.z = f * (deltaUV2.y * edge1.z - deltaUV1.y * edge2.z);
        
        glm::vec3 bitangent;
        bitangent.x = f * (-deltaUV2.x * edge1.x + deltaUV1.x * edge2.x);
        bitangent.y = f * (-deltaUV2.x * edge1.y + deltaUV1.x * edge2.y);
        bitangent.z = f * (-deltaUV2.x * edge1.z + deltaUV1.x * edge2.z);
        
        // Assign to vertices
        v0.tangent = v1.tangent = v2.tangent = tangent;
        v0.bitangent = v1.bitangent = v2.bitangent = bitangent;
    }
    
    // Re-upload the vertex data to the GPU
    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    glBufferData(GL_ARRAY_BUFFER, m_vertices.size() * sizeof(Vertex), m_vertices.data(), GL_STATIC_DRAW);
}

void Mesh::setupMesh() {
    // Clean up previous resources if they exist
    if (m_VAO != 0) {
        glDeleteVertexArrays(1, &m_VAO);
        glDeleteBuffers(1, &m_VBO);
        glDeleteBuffers(1, &m_EBO);
    }
    
    // Create buffers/arrays
    glGenVertexArrays(1, &m_VAO);
    glGenBuffers(1, &m_VBO);
    glGenBuffers(1, &m_EBO);
    
    // Bind VAO first
    glBindVertexArray(m_VAO);
    
    // Load data into vertex buffer
    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    glBufferData(GL_ARRAY_BUFFER, m_vertices.size() * sizeof(Vertex), m_vertices.data(), GL_STATIC_DRAW);
    
    // Load data into index buffer
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indices.size() * sizeof(unsigned int), m_indices.data(), GL_STATIC_DRAW);
    
    // Set vertex attribute pointers
    
    // Position attribute
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));
    
    // Normal attribute
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
    
    // Texture coordinate attribute
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texCoord));
    
    // Tangent attribute
    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, tangent));
    
    // Bitangent attribute
    glEnableVertexAttribArray(4);
    glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, bitangent));
    
    // Unbind VAO
    glBindVertexArray(0);
}

glm::mat4 Mesh::getModelMatrix() const {
    glm::mat4 model = glm::mat4(1.0f);
    
    // Apply transformations in correct order: scale, rotate, translate
    model = glm::translate(model, m_position);
    model = glm::rotate(model, m_rotationAngle, m_rotationAxis);
    model = glm::scale(model, m_scale);
    
    return model;
}

void Mesh::render(Shader& shader) {
    // Apply transformation
    shader.setMat4("model", getModelMatrix());
    
    // Bind VAO and draw elements
    glBindVertexArray(m_VAO);
    glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(m_indices.size()), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

} // namespace rendering
} // namespace engine
