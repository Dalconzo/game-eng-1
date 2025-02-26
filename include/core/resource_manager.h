#pragma once

#include <string>
#include <unordered_map>
#include <memory>
#include <filesystem>
#include "rendering/texture.h"

namespace engine {
namespace core {

class ResourceManager {
public:
    // Initialize the resource manager
    static void init(const std::string& customRootPath = "");
    
    // Get the root directory of the application
    static std::filesystem::path getRootPath();
    
    // Resolve a path relative to the content directory
    static std::filesystem::path resolvePath(const std::string& relativePath);
    
    // Check if a file exists
    static bool fileExists(const std::filesystem::path& path);
    
    // Texture resources
    static std::shared_ptr<rendering::Texture> getTexture(const std::string& relativePath);
    static void unloadTexture(const std::string& relativePath);
    
    // Resource management
    static void clearAllResources();
    
private:
    // Private constructor - this is a singleton
    ResourceManager() = default;
    
    // Try to detect the root directory automatically
    static std::filesystem::path detectRootDirectory();
    
    static std::filesystem::path s_rootPath;
    static bool s_initialized;
    
    // Resource caches
    static std::unordered_map<std::string, std::shared_ptr<rendering::Texture>> s_textureCache;
};

} // namespace core
} // namespace engine
