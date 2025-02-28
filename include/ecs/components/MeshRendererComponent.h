#pragma once
#include "../Component.h"
#include "rendering/model/model.h"
#include "rendering/model/material.h"
#include "rendering/shader.h"

namespace Engine {
namespace ECS {

class MeshRendererComponent : public Component {
private:
    std::shared_ptr<engine::rendering::Model> m_model;
    std::shared_ptr<engine::rendering::Material> m_material;
    bool m_castShadows = true;
    bool m_receiveShadows = true;

public:
    MeshRendererComponent() = default;
    
    void setModel(std::shared_ptr<engine::rendering::Model> model) {
        m_model = model;
    }
    
    void setMaterial(std::shared_ptr<engine::rendering::Material> material) {
        m_material = material;
    }
    
    std::shared_ptr<engine::rendering::Model> getModel() const {
        return m_model;
    }
    
    std::shared_ptr<engine::rendering::Material> getMaterial() const {
        return m_material;
    }
    
    void setCastShadows(bool cast) {
        m_castShadows = cast;
    }
    
    void setReceiveShadows(bool receive) {
        m_receiveShadows = receive;
    }
    
    bool castsShadows() const {
        return m_castShadows;
    }
    
    bool receivesShadows() const {
        return m_receiveShadows;
    }
    
    void render(engine::rendering::Shader& shader);
    
    // Component interface implementation
    virtual void init() override;
    virtual void update(float deltaTime) override;
};

} // namespace ECS
} // namespace Engine
