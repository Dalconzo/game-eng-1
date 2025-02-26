
#include "rendering/primitive_builder.h"

namespace engine {
namespace rendering {

std::unique_ptr<Mesh> PrimitiveBuilder::createTriangle() {
    // Define vertices for a triangle
    std::vector<Vertex> vertices = {
        // Positions                  // Normal is used for colors here
        Vertex{{-0.5f, -0.5f, 0.0f}, {1.0f, 0.0f, 0.0f}},  // Bottom left
        Vertex{{ 0.5f, -0.5f, 0.0f}, {0.0f, 1.0f, 0.0f}},  // Bottom right
        Vertex{{ 0.0f,  0.5f, 0.0f}, {0.0f, 0.0f, 1.0f}}   // Top
    };
    
    // Define indices (simple case for a triangle)
    std::vector<unsigned int> indices = {0, 1, 2};
    
    // Create and set up the mesh
    auto mesh = std::make_unique<Mesh>();
    mesh->setVertices(vertices, indices);
    
    return mesh;
}

std::unique_ptr<Mesh> PrimitiveBuilder::createQuad() {
    // Define vertices for a quad
    std::vector<Vertex> vertices = {
        // Positions                  // Normal is used for colors here
        Vertex{{-0.5f, -0.5f, 0.0f}, {1.0f, 0.0f, 0.0f}},  // Bottom left
        Vertex{{ 0.5f, -0.5f, 0.0f}, {0.0f, 1.0f, 0.0f}},  // Bottom right
        Vertex{{ 0.5f,  0.5f, 0.0f}, {0.0f, 0.0f, 1.0f}},  // Top right
        Vertex{{-0.5f,  0.5f, 0.0f}, {1.0f, 1.0f, 0.0f}}   // Top left
    };
    
    // Define indices (two triangles to form a quad)
    std::vector<unsigned int> indices = {
        0, 1, 2,  // First triangle
        0, 2, 3   // Second triangle
    };
    
    // Create and set up the mesh
    auto mesh = std::make_unique<Mesh>();
    mesh->setVertices(vertices, indices);
    
    return mesh;
}

std::unique_ptr<Mesh> PrimitiveBuilder::createCube() {
    // Define vertices for a cube
    std::vector<float> rawVertices = {
        // Front face positions
        -0.5f, -0.5f,  0.5f,  // 0
         0.5f, -0.5f,  0.5f,  // 1
         0.5f,  0.5f,  0.5f,  // 2
        -0.5f,  0.5f,  0.5f,  // 3
        // Back face positions
        -0.5f, -0.5f, -0.5f,  // 4
         0.5f, -0.5f, -0.5f,  // 5
         0.5f,  0.5f, -0.5f,  // 6
        -0.5f,  0.5f, -0.5f   // 7
    };
    
    // Convert to structured Vertex objects
    std::vector<Vertex> vertices;
    for (size_t i = 0; i < rawVertices.size(); i += 3) {
        Vertex vertex;
        vertex.position = glm::vec3(rawVertices[i], rawVertices[i+1], rawVertices[i+2]);
        // Set default normal, texCoord, etc. if needed
        vertices.push_back(vertex);
    }
    
    // Define indices for the cube (6 faces, 2 triangles per face)
    std::vector<unsigned int> indices = {
        // Front face
        0, 1, 2, 0, 2, 3,
        // Right face
        1, 5, 6, 1, 6, 2,
        // Back face
        5, 4, 7, 5, 7, 6,
        // Left face
        4, 0, 3, 4, 3, 7,
        // Top face
        3, 2, 6, 3, 6, 7,
        // Bottom face
        4, 5, 1, 4, 1, 0
    };
    
    // Create the mesh using the existing interface
    auto mesh = std::make_unique<Mesh>();
    mesh->setVertices(vertices, indices);
    
    return mesh;
}

} // namespace rendering
} // namespace engine
