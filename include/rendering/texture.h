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
    
private:
    unsigned int m_textureID;
    int m_width;
    int m_height;
    int m_channels;
};

} // namespace rendering
} // namespace engine
