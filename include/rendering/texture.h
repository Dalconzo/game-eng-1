#pragma once

#include <string>
#include <GL/glew.h>

namespace engine {
namespace rendering {

class Texture {
public:
    Texture();
    ~Texture();
    
    // Prevent copying
    Texture(const Texture&) = delete;
    Texture& operator=(const Texture&) = delete;
    
    // Load texture from file
    bool loadFromFile(const std::string& filePath);
    
    // Bind the texture to the specified texture unit
    void bind(unsigned int textureUnit = 0) const;
    
    // Getters
    unsigned int getID() const { return m_textureID; }
    int getWidth() const { return m_width; }
    int getHeight() const { return m_height; }
    int getChannels() const { return m_channels; }

    // Add this to include/rendering/texture.h in the public section
    bool createFromData(const unsigned char* data, int width, int height, int channels) {
        // Clean up previous texture if exists
        if (m_textureID != 0) {
            glDeleteTextures(1, &m_textureID);
        }
        
        // Store dimensions
        m_width = width;
        m_height = height;
        m_channels = channels;
        
        // Generate texture
        glGenTextures(1, &m_textureID);
        glBindTexture(GL_TEXTURE_2D, m_textureID);
        
        // Set texture parameters
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        
        // Set format based on channels
        GLenum format = GL_RGB;
        if (channels == 1) {
            format = GL_RED;
        } else if (channels == 3) {
            format = GL_RGB;
        } else if (channels == 4) {
            format = GL_RGBA;
        }
        
        // Upload data to GPU
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
        
        return true;
    }

private:
    unsigned int m_textureID;
    int m_width;
    int m_height;
    int m_channels;
};

} // namespace rendering
} // namespace engine
