#include "rendering/model/model.h"
#include "core/resource_manager.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <map>
#include <filesystem>

namespace engine {
namespace rendering {

Model::Model() {
}

bool Model::loadFromFile(const std::string& filePath) {
    // Clear existing data
    m_meshes.clear();
    m_materials.clear();
    
    // Get file extension to determine which loader to use
    std::filesystem::path path(filePath);
    std::string extension = path.extension().string();
    
    // Convert to lowercase for comparison
    for (auto& c : extension) {
        c = std::tolower(c);
    }
    
    // Use the appropriate loader based on file extension
    if (extension == ".obj") {
        return loadOBJ(filePath);
    } else if (extension == ".fbx") {
        return loadFBX(filePath);
    } else if (extension == ".gltf" || extension == ".glb") {
        return loadGLTF(filePath);
    } else {
        std::cerr << "Unsupported model format: " << extension << std::endl;
        return false;
    }
}

void Model::render(Shader& shader) {
    std::cout << "Model::render - Meshes count: " << m_meshes.size() << std::endl;
    
    for (size_t i = 0; i < m_meshes.size(); i++) {
        std::cout << "Rendering mesh " << i << std::endl;
        // Apply material if available
        if (i < m_materials.size() && m_materials[i]) {
            m_materials[i]->apply(shader);
        }
        
        // Render the mesh
        m_meshes[i]->render(shader);
    }
}

bool Model::loadOBJ(const std::string& filePath) {
    std::cout << "Loading OBJ model: " << filePath << std::endl;
    
    std::vector<glm::vec3> positions;
    std::vector<glm::vec3> normals;
    std::vector<glm::vec2> texCoords;
    
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    
    // Maps to track unique vertices
    std::map<std::string, unsigned int> uniqueVertices;
    
    // Current material
    int currentMaterialIndex = -1;
    std::string materialLib;
    
    std::ifstream file(filePath);
    if (!file) {
        std::cerr << "Failed to open file: " << filePath << std::endl;
        return false;
    }
    
    std::string line;
    while (std::getline(file, line)) {
        std::istringstream iss(line);
        std::string prefix;
        iss >> prefix;
        
        if (prefix == "v") {
            // Vertex position
            glm::vec3 position;
            iss >> position.x >> position.y >> position.z;
            positions.push_back(position);
        } else if (prefix == "vn") {
            // Vertex normal
            glm::vec3 normal;
            iss >> normal.x >> normal.y >> normal.z;
            normals.push_back(normal);
        } else if (prefix == "vt") {
            // Texture coordinate
            glm::vec2 texCoord;
            iss >> texCoord.x >> texCoord.y;
            // OBJ format has origin at bottom-left, OpenGL expects top-left
            texCoord.y = 1.0f - texCoord.y;
            texCoords.push_back(texCoord);
        } else if (prefix == "f") {
            // Face definition
            std::string vertexData;
            std::vector<unsigned int> faceIndices;
            
            // Process each vertex in the face
            while (iss >> vertexData) {
                // Format check: position/texcoord/normal
                std::istringstream viss(vertexData);
                std::string positionIndex, texCoordIndex, normalIndex;
                
                // Parse the vertex format (p/t/n, p//n, p/t, or p)
                std::getline(viss, positionIndex, '/');
                std::getline(viss, texCoordIndex, '/');
                std::getline(viss, normalIndex, '/');
                
                // Create a unique string identifier for this vertex
                std::string vertexKey = vertexData;
                
                // Check if we've seen this vertex before
                if (uniqueVertices.find(vertexKey) == uniqueVertices.end()) {
                    // New vertex - create and add it
                    Vertex vertex;
                    
                    // OBJ indices are 1-based, convert to 0-based
                    int pIdx = std::stoi(positionIndex) - 1;
                    vertex.position = positions[pIdx];
                    
                    // Only add texture coords if provided
                    if (!texCoordIndex.empty()) {
                        int tIdx = std::stoi(texCoordIndex) - 1;
                        vertex.texCoord = texCoords[tIdx];
                    }
                    
                    // Only add normals if provided
                    if (!normalIndex.empty()) {
                        int nIdx = std::stoi(normalIndex) - 1;
                        vertex.normal = normals[nIdx];
                    }
                    
                    // Add the vertex and record its index
                    uniqueVertices[vertexKey] = static_cast<unsigned int>(vertices.size());
                    vertices.push_back(vertex);
                }
                
                // Add this vertex's index to our face indices
                faceIndices.push_back(uniqueVertices[vertexKey]);
            }
            
            // Triangulate the face (assuming convex)
            for (size_t i = 1; i < faceIndices.size() - 1; i++) {
                indices.push_back(faceIndices[0]);
                indices.push_back(faceIndices[i]);
                indices.push_back(faceIndices[i + 1]);
            }
        } else if (prefix == "mtllib") {
            // Material library
            iss >> materialLib;
        } else if (prefix == "usemtl") {
            // Material use
            std::string materialName;
            iss >> materialName;
            
            // If we've accumulated vertices, create a mesh with the current material
            if (!vertices.empty()) {
                auto mesh = std::make_shared<Mesh>();
                mesh->setVertices(vertices, indices);
                mesh->setName("mesh_" + std::to_string(m_meshes.size()));
                m_meshes.push_back(mesh);
                
                // Find and associate the material with this mesh
                std::shared_ptr<Material> material = core::ResourceManager::getMaterial(materialName);
                if (material) {
                    m_materials.push_back(material);
                } else {
                    // If material isn't found, use a default or placeholder
                    m_materials.push_back(nullptr);
                }
                
                // Reset for next mesh
                vertices.clear();
                indices.clear();
                uniqueVertices.clear();
            }
        }
    }
    
    // Add any remaining vertices as a final mesh
    if (!vertices.empty()) {
        auto mesh = std::make_shared<Mesh>();
        mesh->setVertices(vertices, indices);
        m_meshes.push_back(mesh);
        m_materials.push_back(nullptr); // Placeholder
    }
    
    // Load materials if a material library was specified
    if (!materialLib.empty()) {
        std::filesystem::path objPath(filePath);
        std::filesystem::path mtlPath = objPath.parent_path() / materialLib;
        loadMaterials(mtlPath.string());
    }
    
    std::cout << "Loaded " << vertices.size() << " vertices, " << indices.size() << " indices" << std::endl;
    return !m_meshes.empty();
}

bool Model::loadFBX(const std::string& filePath) {
    std::cerr << "FBX loading not yet implemented" << std::endl;
    return false;
}

bool Model::loadGLTF(const std::string& filePath) {
    std::cerr << "GLTF loading not yet implemented" << std::endl;
    return false;
}

bool Model::loadMaterials(const std::string& mtlFilePath) {
    std::cout << "Loading materials from: " << mtlFilePath << std::endl;
    
    std::ifstream file(mtlFilePath);
    if (!file) {
        std::cerr << "Failed to open material file: " << mtlFilePath << std::endl;
        return false;
    }
    
    std::shared_ptr<Material> currentMaterial = nullptr;
    std::string materialName;
    
    // Get the directory containing the MTL file for relative texture paths
    std::filesystem::path mtlDir = std::filesystem::path(mtlFilePath).parent_path();
    
    std::string line;
    while (std::getline(file, line)) {
        std::istringstream iss(line);
        std::string prefix;
        iss >> prefix;
        
        if (prefix == "newmtl") {
            // New material definition
            iss >> materialName;
            
            // Create a new material using the ResourceManager
            currentMaterial = core::ResourceManager::createMaterial(materialName);
            
            // Store the material for later use
            m_materials.push_back(currentMaterial);
        } else if (prefix == "Ka" && currentMaterial) {
            // Ambient color
            glm::vec3 ambient;
            iss >> ambient.r >> ambient.g >> ambient.b;
            currentMaterial->setAmbient(ambient);
        } else if (prefix == "Kd" && currentMaterial) {
            // Diffuse color
            glm::vec3 diffuse;
            iss >> diffuse.r >> diffuse.g >> diffuse.b;
            currentMaterial->setDiffuse(diffuse);
        } else if (prefix == "Ks" && currentMaterial) {
            // Specular color
            glm::vec3 specular;
            iss >> specular.r >> specular.g >> specular.b;
            currentMaterial->setSpecular(specular);
        } else if (prefix == "Ns" && currentMaterial) {
            // Specular exponent (shininess)
            float shininess;
            iss >> shininess;
            currentMaterial->setShininess(shininess);
        } else if (prefix == "map_Kd" && currentMaterial) {
            // Diffuse texture map
            std::string texturePath;
            iss >> texturePath;
            
            // Resolve the texture path relative to the MTL file
            std::filesystem::path fullTexturePath = mtlDir / texturePath;
            
            // Use the ResourceManager to load the texture
            auto texture = core::ResourceManager::getTexture(fullTexturePath.string());
            if (texture) {
                currentMaterial->setDiffuseMap(texture);
            }
        } else if (prefix == "map_Ks" && currentMaterial) {
            // Specular texture map
            std::string texturePath;
            iss >> texturePath;
            
            std::filesystem::path fullTexturePath = mtlDir / texturePath;
            auto texture = core::ResourceManager::getTexture(fullTexturePath.string());
            if (texture) {
                currentMaterial->setSpecularMap(texture);
            }
        } else if (prefix == "norm" && currentMaterial) {
            // Normal map (or bump map in some formats)
            std::string texturePath;
            iss >> texturePath;
            
            std::filesystem::path fullTexturePath = mtlDir / texturePath;
            auto texture = core::ResourceManager::getTexture(fullTexturePath.string());
            if (texture) {
                currentMaterial->setNormalMap(texture);
            }
        }
    }
    
    std::cout << "Loaded " << m_materials.size() << " materials" << std::endl;
    return !m_materials.empty();
}
} // namespace rendering
} // namespace engine
