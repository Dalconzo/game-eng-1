#include "core/resources/material_manager.h"
#include <iostream>

namespace engine {
namespace core {
namespace resources {

MaterialManager::MaterialManager() {
    std::cout << "Material manager initialized" << std::endl;
}

MaterialManager::~MaterialManager() {
    // Base class handles cleanup
}

std::shared_ptr<rendering::Material> MaterialManager::getMaterial(const std::string& name) {
    return getResource(name);
}

std::shared_ptr<rendering::Material> MaterialManager::createMaterial(const std::string& name) {
    // Check if material already exists
    auto material = getResource(name);
    if (material) {
        return material;
    }
    
    // Create new material
    material = std::make_shared<rendering::Material>();
    
    // Add to cache
    addResource(name, material);
    std::cout << "Created and cached material: " << name << std::endl;
    
    return material;
}

bool MaterialManager::unloadMaterial(const std::string& name) {
    return unloadResource(name);
}

} // namespace resources
} // namespace core
} // namespace engine
