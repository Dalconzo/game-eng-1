#include "rendering/texture.h"
#include <iostream>
#include <GL/glew.h>
#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"

namespace engine {
namespace rendering {

Texture::Texture() : m_textureID(0), m_width(0), m_height(0), m_channels(0) {
}

Texture::~Texture() {
    if (m_textureID != 0) {
        glDeleteTextures(1, &m_textureID);
    }
}

bool Texture::loadFromFile(const std::string& filePath) {
    // Clean up existing texture if any
    if (m_textureID != 0) {
        glDeleteTextures(1, &m_textureID);
        m_textureID = 0;
    }
    
    // Load image data
    stbi_set_flip_vertically_on_load(true);
    unsigned char* data = stbi_load(filePath.c_str(), &m_width, &m_height, &m_channels, 0);
    
    if (!data) {
        std::cerr << "Failed to load texture: " << filePath << std::endl;
        std::cerr << "STB Image error: " << stbi_failure_reason() << std::endl;
        return false;
    }
    
    // Create OpenGL texture
    glGenTextures(1, &m_textureID);
    glBindTexture(GL_TEXTURE_2D, m_textureID);
    
    // Set texture wrapping/filtering options
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    
    // Upload texture data
    GLenum format = GL_RGB;
    if (m_channels == 1) {
        format = GL_RED;
    } else if (m_channels == 3) {
        format = GL_RGB;
    } else if (m_channels == 4) {
        format = GL_RGBA;
    }
    
    glTexImage2D(GL_TEXTURE_2D, 0, format, m_width, m_height, 0, format, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
    
    // Free image data
    stbi_image_free(data);
    
    return true;
}

void Texture::bind(unsigned int textureUnit) const {
    glActiveTexture(GL_TEXTURE0 + textureUnit);
    glBindTexture(GL_TEXTURE_2D, m_textureID);
}



} // namespace rendering
} // namespace engine
