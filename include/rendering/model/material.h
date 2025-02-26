#pragma once

#include "rendering/texture.h"
#include "rendering/shader.h"
#include <memory>
#include <string>
#include <glm/glm.hpp>

namespace engine {
namespace rendering {

class Material {
public:
    Material();
    ~Material() = default;
    
    // Apply material properties to a shader
    void apply(Shader& shader) const;
    
    // Texture maps
    void setDiffuseMap(std::shared_ptr<Texture> texture) { m_diffuseMap = texture; }
    void setSpecularMap(std::shared_ptr<Texture> texture) { m_specularMap = texture; }
    void setNormalMap(std::shared_ptr<Texture> texture) { m_normalMap = texture; }
    
    // Material properties
    void setAmbient(const glm::vec3& ambient) { m_ambient = ambient; }
    void setDiffuse(const glm::vec3& diffuse) { m_diffuse = diffuse; }
    void setSpecular(const glm::vec3& specular) { m_specular = specular; }
    void setShininess(float shininess) { m_shininess = shininess; }
    
    // Getters
    const glm::vec3& getAmbient() const { return m_ambient; }
    const glm::vec3& getDiffuse() const { return m_diffuse; }
    const glm::vec3& getSpecular() const { return m_specular; }
    float getShininess() const { return m_shininess; }
    bool hasDiffuseMap() const { return m_diffuseMap != nullptr; }
    bool hasSpecularMap() const { return m_specularMap != nullptr; }
    bool hasNormalMap() const { return m_normalMap != nullptr; }
    
private:
    // Material textures
    std::shared_ptr<Texture> m_diffuseMap;
    std::shared_ptr<Texture> m_specularMap;
    std::shared_ptr<Texture> m_normalMap;
    
    // Material properties
    glm::vec3 m_ambient;
    glm::vec3 m_diffuse;
    glm::vec3 m_specular;
    float m_shininess;
};

} // namespace rendering
} // namespace engine
