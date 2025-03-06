#include "ecs/ECSManager.h"
#include "core/resource_manager.h"
#include "rendering/window.h"
#include "ecs/components/CameraControllerComponent.h"
#include <GLFW/glfw3.h>
#include <iostream>

namespace Engine {
namespace ECS {

engine::rendering::Window* ECSManager::window = nullptr;

ECSManager::ECSManager() {
    // Initialize free entity IDs
    for (EntityID i = 0; i < MAX_ENTITIES; i++) {
        freeEntities.push_back(i);
    }
}

Entity* ECSManager::createEntity() {
    if (freeEntities.empty()) {
        throw std::runtime_error("Maximum number of entities reached");
    }
    
    EntityID id = freeEntities.front();
    freeEntities.erase(freeEntities.begin());
    livingEntityCount++;
    
    entities[id] = std::make_unique<Entity>(id, this);
    return entities[id].get();
}

void ECSManager::destroyEntity(EntityID id) {
    if (id >= MAX_ENTITIES || !entities[id]) {
        return;
    }
    
    entities[id]->destroy();
}

Entity* ECSManager::getEntity(EntityID id) {
    if (id >= MAX_ENTITIES) {
        return nullptr;
    }
    return entities[id].get();
}

void ECSManager::update(float deltaTime) {
    std::cout << "ECSManager: update starting" << std::endl;
    for (auto& system : systems) {
        if (system->isActive()) {
            system->update(deltaTime);
        }
    }
    std::cout << "ECSManager: update completed" << std::endl;
}

void ECSManager::render() {
    std::cout << "ECSManager: render starting" << std::endl;
    int width = window->getWidth();
    int height = window->getHeight();
    glViewport(0, 0, width, height);
    for (auto& system : systems) {
        if (system->isActive()) {
            system->render();
        }
    }
    std::cout << "ECSManager: render completed" << std::endl;
}

void ECSManager::refresh() {
    for (std::size_t i = 0; i < MAX_ENTITIES; i++) {
        if (entities[i] && !entities[i]->isActive()) {
            // Remove entity from all systems
            for (auto& system : systems) {
                system->removeEntity(entities[i].get());
            }
            
            // Reset components
            for (std::size_t j = 0; j < MAX_COMPONENTS; j++) {
                if (components[j].size() > i && components[j][i]) {
                    components[j][i].reset();
                }
            }
            
            // Free entity ID
            entities[i].reset();
            freeEntities.push_back(static_cast<EntityID>(i));
            livingEntityCount--;
        }
    }
}

void ECSManager::initialize(engine::rendering::Window* gameWindow) {
    window = gameWindow;
    
    // Set window resize callback to update any camera components
    window->setResizeCallback([this](int width, int height) {
        // Update projection matrices of all camera components
        // We'll implement this with the camera component
    });
    
    ::engine::core::ResourceManager::init();
}

void ECSManager::runGameLoop() {
    if (!window) {
        throw std::runtime_error("Window not initialized in ECSManager");
    }
    
    auto lastTime = std::chrono::high_resolution_clock::now();

    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    
    // Main game loop / render loop
    while (!window->shouldClose()) {
        std::cout << "Starting game loop" << std::endl;
        // Calculate delta time
        auto currentTime = std::chrono::high_resolution_clock::now();
        float deltaTime = std::chrono::duration<float>(currentTime - lastTime).count();
        lastTime = currentTime;
        std::cout << "Processing input" << std::endl;
        
        // Process input
        processInput(deltaTime);

        std::cout << "Updating systems" << std::endl;
        
        // Update all active systems
        update(deltaTime);

        std::cout << "Rendering frame" << std::endl;
        
        // Render frame
        render();
        
        // Clear inactive entities
        refresh();
        
        // Swap buffers and poll events
        window->swapBuffers();
        window->pollEvents();
        // window->close();
    }
}

void ECSManager::processInput(float deltaTime) {
    if (!window) {
        return;
    }
    
    // Process camera controller input for all camera controller entities
    for (auto entity : getAllEntities()) {
        if (entity && entity->isActive() && 
            entity->hasComponent<CameraControllerComponent>()) {
            auto& controller = entity->getComponent<CameraControllerComponent>();
            controller.handleInput(window);
        }
    }
}

void ECSManager::shutdown() {
    // Clean up all entities and systems
    for (auto& system : systems) {
        system.reset();
    }
    systems.clear();
    
    // Reset all entities
    for (std::size_t i = 0; i < MAX_ENTITIES; i++) {
        if (entities[i]) {
            entities[i].reset();
        }
    }
    
    // Reset components
    for (std::size_t i = 0; i < MAX_COMPONENTS; i++) {
        components[i].clear();
    }
    
    // Reset free entities list
    freeEntities.clear();
    for (EntityID i = 0; i < MAX_ENTITIES; i++) {
        freeEntities.push_back(i);
    }
    livingEntityCount = 0;
}

// Resource loading methods to simplify component access
std::shared_ptr<engine::rendering::Texture> ECSManager::loadTexture(const std::string& path) {
    return ::engine::core::ResourceManager::getTexture(path);
}

std::shared_ptr<engine::rendering::Model> ECSManager::loadModel(const std::string& path) {
    return ::engine::core::ResourceManager::getModel(path);
}

std::shared_ptr<engine::rendering::Shader> ECSManager::loadShader(
    const std::string& name,
    const std::string& vertexPath,
    const std::string& fragmentPath) {
    
    return ::engine::core::ResourceManager::getShader(name, vertexPath, fragmentPath);
}

std::shared_ptr<engine::rendering::Material> ECSManager::createMaterial(const std::string& name) {
    return ::engine::core::ResourceManager::createMaterial(name);
}

std::vector<Entity*> ECSManager::getAllEntities() const {
    std::vector<Entity*> result;
    for (std::size_t i = 0; i < MAX_ENTITIES; i++) {
        if (entities[i]) {
            std::cout << "Found valid entity ID: " << i << std::endl;
            result.push_back(entities[i].get());
        }
    }
    std::cout << "getAllEntities found " << result.size() << " entities" << std::endl;
    return result;
}
} // namespace ECS
} // namespace Engine