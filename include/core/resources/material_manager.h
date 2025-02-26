#pragma once

#include "core/resources/base_resource_manager.h"
#include "rendering/model/material.h"

namespace engine {
namespace core {
namespace resources {

class MaterialManager : public BaseResourceManager<rendering::Material> {
public:
    MaterialManager();
    ~MaterialManager() override;
    
    // Material-specific operations
    std::shared_ptr<rendering::Material> getMaterial(const std::string& name);
    std::shared_ptr<rendering::Material> createMaterial(const std::string& name);
    bool unloadMaterial(const std::string& name);
};

} // namespace resources
} // namespace core
} // namespace engine
