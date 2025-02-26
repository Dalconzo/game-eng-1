#pragma once

#include "core/resources/base_resource_manager.h"
#include "rendering/model/model.h"

namespace engine {
namespace core {
namespace resources {

class ModelManager : public BaseResourceManager<rendering::Model> {
public:
    ModelManager();
    ~ModelManager() override;
    
    // Model-specific operations
    std::shared_ptr<rendering::Model> getModel(const std::string& filePath);
    bool unloadModel(const std::string& filePath);
};

} // namespace resources
} // namespace core
} // namespace engine
