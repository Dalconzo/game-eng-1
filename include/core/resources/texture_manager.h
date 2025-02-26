#pragma once

#include "core/resources/base_resource_manager.h"
#include "rendering/texture.h"

namespace engine {
namespace core {
namespace resources {

class TextureManager : public BaseResourceManager<rendering::Texture> {
public:
    TextureManager();
    ~TextureManager() override;
    
    // Texture-specific operations
    std::shared_ptr<rendering::Texture> getTexture(const std::string& filePath);
    bool unloadTexture(const std::string& filePath);
};

} // namespace resources
} // namespace core
} // namespace engine
