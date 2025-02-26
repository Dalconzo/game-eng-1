#include "core/resources/model_manager.h"
#include <iostream>

namespace engine {
namespace core {
namespace resources {

ModelManager::ModelManager() {
    std::cout << "Model manager initialized" << std::endl;
}

ModelManager::~ModelManager() {
    // Base class handles cleanup
}

std::shared_ptr<rendering::Model> ModelManager::getModel(const std::string& filePath) {
    // Check cache first
    auto model = getResource(filePath);
    if (model) {
        return model;
    }
    
    // Create and load the model
    model = std::make_shared<rendering::Model>();
    if (!model->loadFromFile(filePath)) {
        std::cerr << "Failed to load model: " << filePath << std::endl;
        return nullptr;
    }
    
    // Add to cache
    addResource(filePath, model);
    std::cout << "Loaded and cached model: " << filePath << std::endl;
    
    // Note: In a real implementation, we would register dependencies on materials and textures here
    
    return model;
}

bool ModelManager::unloadModel(const std::string& filePath) {
    return unloadResource(filePath);
}

} // namespace resources
} // namespace core
} // namespace engine
