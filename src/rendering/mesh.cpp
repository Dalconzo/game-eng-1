#include "rendering/mesh.h"
#include <GL/glew.h>
#include <glm/gtc/matrix_transform.hpp>

namespace engine {
namespace rendering {

Mesh::Mesh(const std::vector<float>& vertices, int positionComponents, int colorComponents) {
    m_stride = (positionComponents + colorComponents) * sizeof(float);
    m_vertexCount = static_cast<int>(vertices.size()) / (positionComponents + colorComponents);
    
    // Create VAO and VBO
    glGenVertexArrays(1, &m_vao);
    glGenBuffers(1, &m_vbo);
    
    glBindVertexArray(m_vao);
    
    // Load vertex data
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);
    
    // Set up position attribute
    glVertexAttribPointer(0, positionComponents, GL_FLOAT, GL_FALSE, m_stride, (void*)0);
    glEnableVertexAttribArray(0);
    
    // Set up color attribute
    glVertexAttribPointer(1, colorComponents, GL_FLOAT, GL_FALSE, m_stride, 
                         (void*)(positionComponents * sizeof(float)));
    glEnableVertexAttribArray(1);
    
    // Unbind
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

Mesh::~Mesh() {
    glDeleteVertexArrays(1, &m_vao);
    glDeleteBuffers(1, &m_vbo);
}

void Mesh::render(Shader& shader) {
    shader.use();
    
    // Set model matrix
    shader.setMat4("model", getModelMatrix());
    
    // Draw the mesh
    glBindVertexArray(m_vao);
    glDrawArrays(GL_TRIANGLES, 0, m_vertexCount);
    glBindVertexArray(0);
}

glm::mat4 Mesh::getModelMatrix() const {
    glm::mat4 model = glm::mat4(1.0f);
    
    // Apply transformations in order: scale, rotate, translate
    model = glm::translate(model, m_position);
    model = glm::rotate(model, glm::radians(m_rotationAngle), m_rotationAxis);
    model = glm::scale(model, m_scale);
    
    return model;
}

} // namespace rendering
} // namespace engine
