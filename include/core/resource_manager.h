#pragma once

#include <string>
#include <filesystem>
#include <memory>

#include "rendering/texture.h"
#include "rendering/model/model.h"
#include "rendering/shader.h"

// Forward declarations of resource handlers
namespace engine {
namespace core {
namespace resources {
    class TextureManager;
    class ModelManager;
    class ShaderManager;
    class MaterialManager;
}
}
}

namespace engine {
namespace core {

class ResourceManager {
public:
    // Initialize the resource manager
    static void init(const std::string& customRootPath = "");
    
    // Shutdown and cleanup
    static void shutdown();
    
    // Path utilities
    static std::filesystem::path getRootPath();
    static std::filesystem::path resolvePath(const std::string& relativePath);
    static bool fileExists(const std::filesystem::path& path);
    
    // Resource access - these delegate to the specialized managers
    static std::shared_ptr<rendering::Texture> getTexture(const std::string& relativePath);
    static bool unloadTexture(const std::string& relativePath);
    
    static std::shared_ptr<rendering::Model> getModel(const std::string& relativePath);
    static bool unloadModel(const std::string& relativePath);
    
    static std::shared_ptr<rendering::Shader> getShader(const std::string& name, 
                                                       const std::string& vertexPath,
                                                       const std::string& fragmentPath);
    static bool unloadShader(const std::string& name);
    
    // Clear all resources
    static void clearAllResources();
    
    // Material resources
    static std::shared_ptr<rendering::Material> getMaterial(const std::string& name);
    static std::shared_ptr<rendering::Material> createMaterial(const std::string& name);
    static bool unloadMaterial(const std::string& name);

private:
    // Private constructor - this is a singleton
    ResourceManager() = default;
    
    // Detect root directory automatically
    static std::filesystem::path detectRootDirectory();
    
    // Path management
    static std::filesystem::path s_rootPath;
    static bool s_initialized;
    
    // Resource handlers
    static std::unique_ptr<resources::TextureManager> s_textureManager;
    static std::unique_ptr<resources::ModelManager> s_modelManager;
    static std::unique_ptr<resources::ShaderManager> s_shaderManager;
    static std::unique_ptr<resources::MaterialManager> s_materialManager;
};

} // namespace core
} // namespace engine

