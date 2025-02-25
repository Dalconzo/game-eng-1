#include "core/resource_manager.h"
#include <filesystem>
#include <iostream>

namespace engine {
namespace core {

std::string ResourceManager::s_projectRoot = "";

void ResourceManager::init() {
    s_projectRoot = findProjectRoot();
    std::cout << "Project root detected as: " << s_projectRoot << std::endl;
}

//void ResourceManager::init(const std::string& projectRoot) {
//    s_projectRoot = projectRoot;
//    std::cout << "Project root set to: " << s_projectRoot << std::endl;
//}

std::string ResourceManager::resolvePath(const std::string& relativePath) {
    return (std::filesystem::path(s_projectRoot) / relativePath).string();
}

std::string ResourceManager::findProjectRoot() {
    std::filesystem::path currentPath = std::filesystem::current_path();
    
    // Look for known project directories/files to identify the root
    while (!currentPath.empty()) {
        if (std::filesystem::exists(currentPath / "assets" / "shaders")) {
            return currentPath.string();
        }
        // Go up one directory
        auto parentPath = currentPath.parent_path();
        if (parentPath == currentPath) break; // We've reached the filesystem root
        currentPath = parentPath;
    }
    
    // Fallback to current directory
    return std::filesystem::current_path().string();
}

} // namespace core
} // namespace engine
