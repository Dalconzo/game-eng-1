#include "core/resources/shader_manager.h"
#include <iostream>

namespace engine {
namespace core {
namespace resources {

ShaderManager::ShaderManager() {
    std::cout << "Shader manager initialized" << std::endl;
}

ShaderManager::~ShaderManager() {
    // Base class handles cleanup
}

std::shared_ptr<rendering::Shader> ShaderManager::getShader(
    const std::string& name,
    const std::string& vertexPath,
    const std::string& fragmentPath) {
    
    // Check if shader is already loaded
    auto shader = getResource(name);
    if (shader) {
        return shader;
    }
    
    // Create and load the shader
    shader = std::make_shared<rendering::Shader>();
    if (!shader->loadFromFiles(vertexPath, fragmentPath)) {
        std::cerr << "Failed to load shader: " << name << std::endl;
        return nullptr;
    }
    
    // Cache the shader
    addResource(name, shader);
    std::cout << "Loaded and cached shader: " << name << std::endl;
    
    return shader;
}

bool ShaderManager::unloadShader(const std::string& name) {
    return unloadResource(name);
}

} // namespace resources
} // namespace core
} // namespace engine
