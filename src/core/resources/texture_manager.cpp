#include "core/resources/texture_manager.h"
#include <iostream>

namespace engine {
namespace core {
namespace resources {

TextureManager::TextureManager() {
    std::cout << "Texture manager initialized" << std::endl;
}

TextureManager::~TextureManager() {

}

std::shared_ptr<rendering::Texture> TextureManager::getTexture(const std::string& filePath) {
    // First check if we already have this texture
    auto texture = getResource(filePath);
    if (texture) {
        return texture;
    }
    
    // Create and load the texture
    texture = std::make_shared<rendering::Texture>();
    if (!texture->loadFromFile(filePath)) {
        std::cerr << "Failed to load texture: " << filePath << std::endl;
        return nullptr;
    }
    
    // Cache the texture
    addResource(filePath, texture);
    std::cout << "Loaded and cached texture: " << filePath << std::endl;
    
    return texture;
}

bool TextureManager::unloadTexture(const std::string& filePath) {
    return unloadResource(filePath);
}

} // namespace resources
} // namespace core
} // namespace engine
