#include "core/resource_manager.h"
#include "core/resources/texture_manager.h"
#include "core/resources/model_manager.h"
#include "core/resources/shader_manager.h"
#include "core/resources/material_manager.h"
#include <iostream>
#include <filesystem>

namespace engine {
namespace core {

std::filesystem::path ResourceManager::s_rootPath;
bool ResourceManager::s_initialized = false;
std::unique_ptr<resources::TextureManager> ResourceManager::s_textureManager = nullptr;
std::unique_ptr<resources::ModelManager> ResourceManager::s_modelManager = nullptr;
std::unique_ptr<resources::ShaderManager> ResourceManager::s_shaderManager = nullptr;
std::unique_ptr<resources::MaterialManager> ResourceManager::s_materialManager = nullptr;

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
    
    s_textureManager = std::make_unique<resources::TextureManager>();
    s_modelManager = std::make_unique<resources::ModelManager>();
    s_shaderManager = std::make_unique<resources::ShaderManager>();
    s_materialManager = std::make_unique<resources::MaterialManager>();
    
    s_initialized = true;
}

void ResourceManager::shutdown() {
    if (!s_initialized) {
        return;
    }
    
    s_textureManager->clearAll();
    s_modelManager->clearAll();
    s_shaderManager->clearAll();
    s_materialManager->clearAll();
    
    s_textureManager.reset();
    s_modelManager.reset();
    s_shaderManager.reset();
    s_materialManager.reset();
    
    s_initialized = false;
    std::cout << "Resource manager shutdown complete" << std::endl;
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

std::shared_ptr<rendering::Material> ResourceManager::getMaterial(const std::string& name) {
    if (!s_initialized) {
        init();
    }
    
    return s_materialManager->getMaterial(name);
}

std::shared_ptr<rendering::Material> ResourceManager::createMaterial(const std::string& name) {
    if (!s_initialized) {
        init();
    }
    
    return s_materialManager->createMaterial(name);
}

bool ResourceManager::unloadMaterial(const std::string& name) {
    if (!s_initialized) {
        return false;
    }
    
    return s_materialManager->unloadMaterial(name);
}

bool ResourceManager::fileExists(const std::filesystem::path& path) {
    return std::filesystem::exists(path) && std::filesystem::is_regular_file(path);
}

std::shared_ptr<rendering::Texture> ResourceManager::getTexture(const std::string& relativePath) {
    if (!s_initialized) {
        init();
    }
    
    std::filesystem::path fullPath = resolvePath(relativePath);
    return s_textureManager->getTexture(fullPath.string());
}

bool ResourceManager::unloadTexture(const std::string& relativePath) {
    if (!s_initialized) {
        return false;
    }
    
    std::filesystem::path fullPath = resolvePath(relativePath);
    return s_textureManager->unloadTexture(fullPath.string());
}

std::shared_ptr<rendering::Model> ResourceManager::getModel(const std::string& relativePath) {
    if (!s_initialized) {
        init();
    }
    
    std::filesystem::path fullPath = resolvePath(relativePath);
    return s_modelManager->getModel(fullPath.string());
}

bool ResourceManager::unloadModel(const std::string& relativePath) {
    if (!s_initialized) {
        return false;
    }
    
    std::filesystem::path fullPath = resolvePath(relativePath);
    return s_modelManager->unloadModel(fullPath.string());
}

std::shared_ptr<rendering::Shader> ResourceManager::getShader(
    const std::string& name, 
    const std::string& vertexPath,
    const std::string& fragmentPath) {
    
    if (!s_initialized) {
        init();
    }
    
    std::filesystem::path vertFullPath = resolvePath(vertexPath);
    std::filesystem::path fragFullPath = resolvePath(fragmentPath);
    
    return s_shaderManager->getShader(name, vertFullPath.string(), fragFullPath.string());
}

bool ResourceManager::unloadShader(const std::string& name) {
    if (!s_initialized) {
        return false;
    }
    
    return s_shaderManager->unloadShader(name);
}

void ResourceManager::clearAllResources() {
    if (!s_initialized) {
        return;
    }
    
    s_textureManager->clearAll();
    s_modelManager->clearAll();
    s_shaderManager->clearAll();
    
    std::cout << "Cleared all cached resources" << std::endl;
}

std::filesystem::path ResourceManager::detectRootDirectory() {
    std::filesystem::path currentPath = std::filesystem::current_path();
    std::filesystem::path originalPath = currentPath;
    
    const std::vector<std::string> markers = {
        "assets",
        "resources",
        "data",
        "src"
    };
    
    while (true) {
        for (const auto& marker : markers) {
            if (std::filesystem::exists(currentPath / marker)) {
                return currentPath;
            }
        }
        
        if (currentPath.parent_path() == currentPath) {
            break;
        }
        
        currentPath = currentPath.parent_path();
    }
    
    std::cout << "Warning: Could not detect project root directory. Using current directory." << std::endl;
    return originalPath;
}

static std::filesystem::path resolvePathWithContext(const std::string& relativePath, 
                                                   const std::filesystem::path& contextPath) {
    // If path is absolute, use it directly
    if (std::filesystem::path(relativePath).is_absolute()) {
        return relativePath;
    }
    
    // Check if path is relative to context
    std::filesystem::path contextRelative = contextPath.parent_path() / relativePath;
    if (ResourceManager::fileExists(contextRelative)) {
        return contextRelative;
    }
    
    // Fall back to root-relative path
    return ResourceManager::resolvePath(relativePath);
}
} // namespace core
} // namespace engine


