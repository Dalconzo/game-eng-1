#pragma once

#include "rendering/mesh.h"
#include "rendering/texture.h"
#include "rendering/model/material.h"
#include "rendering/shader.h"
#include <vector>
#include <memory>
#include <string>
#include <glm/glm.hpp>

namespace engine {
namespace rendering {

class Model {
public:
    Model();
    ~Model() = default;
    
    // Load model from file
    bool loadFromFile(const std::string& filePath);
    
    // Render the model
    void render(Shader& shader);
    
    // Getters
    const std::vector<std::shared_ptr<Mesh>>& getMeshes() const { return m_meshes; }
    
private:
    // Model data
    std::vector<std::shared_ptr<Mesh>> m_meshes;
    std::vector<std::shared_ptr<Material>> m_materials;
    
    // Helper methods for different file formats
    bool loadOBJ(const std::string& filePath);
    bool loadFBX(const std::string& filePath);
    bool loadGLTF(const std::string& filePath);
    
    // Material loading
    bool loadMaterials(const std::string& mtlFilePath);
    
    // Process materials and textures
    void processMaterials(const std::string& directory);
};

} // namespace rendering
} // namespace engine
