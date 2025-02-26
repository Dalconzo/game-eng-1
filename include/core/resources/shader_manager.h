#pragma once

#include "core/resources/base_resource_manager.h"
#include "rendering/shader.h"

namespace engine {
namespace core {
namespace resources {

class ShaderManager : public BaseResourceManager<rendering::Shader> {
public:
    ShaderManager();
    ~ShaderManager() override;

    // Shader-specific operations
    std::shared_ptr<rendering::Shader> getShader(const std::string& name, 
                                                const std::string& vertexPath,
                                                const std::string& fragmentPath);
    bool unloadShader(const std::string& name);
};

} // namespace resources
} // namespace core
} // namespace engine
