#include "rendering/model/material.h"

namespace engine {
namespace rendering {

Material::Material()
    : m_ambient(0.2f, 0.2f, 0.2f)
    , m_diffuse(0.8f, 0.8f, 0.8f)
    , m_specular(1.0f, 1.0f, 1.0f)
    , m_shininess(32.0f)
{
}

void Material::apply(Shader& shader) const {
    // Set material properties in the shader
    shader.setVec3("material.ambient", m_ambient);
    shader.setVec3("material.diffuse", m_diffuse);
    shader.setVec3("material.specular", m_specular);
    shader.setFloat("material.shininess", m_shininess);
    
    // Set texture usage flags
    shader.setInt("material.hasDiffuseMap", hasDiffuseMap() ? 1 : 0);
    shader.setInt("material.hasSpecularMap", hasSpecularMap() ? 1 : 0);
    shader.setInt("material.hasNormalMap", hasNormalMap() ? 1 : 0);
    
    // Bind textures if available
    if (m_diffuseMap) {
        // Activate texture unit 0
        glActiveTexture(GL_TEXTURE0);
        // Bind the diffuse texture
        m_diffuseMap->bind();
        // Tell the shader to use texture unit 0 for the diffuse map
        shader.setInt("material.diffuseMap", 0);
    }
    
    if (m_specularMap) {
        // Activate texture unit 1
        glActiveTexture(GL_TEXTURE1);
        // Bind the specular texture
        m_specularMap->bind();
        // Tell the shader to use texture unit 1 for the specular map
        shader.setInt("material.specularMap", 1);
    }
    
    if (m_normalMap) {
        // Activate texture unit 2
        glActiveTexture(GL_TEXTURE2);
        // Bind the normal texture
        m_normalMap->bind();
        // Tell the shader to use texture unit 2 for the normal map
        shader.setInt("material.normalMap", 2);
    }
}

} // namespace rendering
} // namespace engine
