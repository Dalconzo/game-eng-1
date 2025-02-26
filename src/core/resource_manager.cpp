#include "core/resource_manager.h"
#include <iostream>

namespace engine {
namespace core {

std::filesystem::path ResourceManager::s_rootPath;
bool ResourceManager::s_initialized = false;
std::unordered_map<std::string, std::shared_ptr<rendering::Texture>> ResourceManager::s_textureCache;

void ResourceManager::init(const std::string& customRootPath) {
    if (s_initialized) {
        return;
    }
    
    if (!customRootPath.empty()) {
        s_rootPath = std::filesystem::absolute(customRootPath);
    } else {
        s_rootPath = detectRootDirectory();
    }
    
    std::cout << "Resource root path: " << s_rootPath << std::endl;
    s_initialized = true;
}

std::filesystem::path ResourceManager::getRootPath() {
    if (!s_initialized) {
        init();
    }
    return s_rootPath;
}

std::filesystem::path ResourceManager::resolvePath(const std::string& relativePath) {
    if (!s_initialized) {
        init();
    }
    
    std::filesystem::path fullPath = s_rootPath / relativePath;
    
    if (!fileExists(fullPath)) {
        std::cerr << "Warning: File not found: " << fullPath << std::endl;
    }
    
    return fullPath;
}

bool ResourceManager::fileExists(const std::filesystem::path& path) {
    return std::filesystem::exists(path) && std::filesystem::is_regular_file(path);
}

std::shared_ptr<rendering::Texture> ResourceManager::getTexture(const std::string& relativePath) {
    // Check if texture is already loaded
    auto it = s_textureCache.find(relativePath);
    if (it != s_textureCache.end()) {
        return it->second;
    }
    
    // Resolve the absolute path
    std::filesystem::path fullPath = resolvePath(relativePath);
    
    // Create and load the texture
    std::shared_ptr<rendering::Texture> texture = std::make_shared<rendering::Texture>();
    if (!texture->loadFromFile(fullPath.string())) {
        std::cerr << "Failed to load texture: " << relativePath << std::endl;
        return nullptr;
    }
    
    // Cache the texture
    s_textureCache[relativePath] = texture;
    std::cout << "Loaded and cached texture: " << relativePath << std::endl;
    
    return texture;
}

void ResourceManager::unloadTexture(const std::string& relativePath) {
    auto it = s_textureCache.find(relativePath);
    if (it != s_textureCache.end()) {
        s_textureCache.erase(it);
        std::cout << "Unloaded texture: " << relativePath << std::endl;
    }
}

void ResourceManager::clearAllResources() {
    s_textureCache.clear();
    std::cout << "Cleared all cached resources" << std::endl;
}

std::filesystem::path ResourceManager::detectRootDirectory() {
    // Start with the current working directory
    std::filesystem::path currentPath = std::filesystem::current_path();
    std::filesystem::path originalPath = currentPath;
    
    // Look for common project directories or markers
    const std::vector<std::string> markers = {
        "assets",
        "resources",
        "data",
        "src"
    };
    
    // Check if any markers exist in the current directory
    while (true) {
        for (const auto& marker : markers) {
            if (std::filesystem::exists(currentPath / marker)) {
                return currentPath;
            }
        }
        
        // If we're at the root directory, we can't go up any further
        if (currentPath.parent_path() == currentPath) {
            break;
        }
        
        // Go up one directory
        currentPath = currentPath.parent_path();
    }
    
    // If we couldn't find a suitable directory, return the original path
    std::cout << "Warning: Could not detect project root directory. Using current directory." << std::endl;
    return originalPath;
}

} // namespace core
} // namespace engine
