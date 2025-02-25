#include "core/game_loop.h"

namespace engine {
namespace core {

GameLoop::GameLoop()
    : m_running(false) {
}

void GameLoop::setUpdateCallback(UpdateCallback callback) {
    m_updateCallback = std::move(callback);
}

void GameLoop::setFixedUpdateCallback(FixedUpdateCallback callback) {
    m_fixedUpdateCallback = std::move(callback);
}

void GameLoop::setRenderCallback(RenderCallback callback) {
    m_renderCallback = std::move(callback);
}

void GameLoop::start() {
    m_running = true;
    
    // Reset time management
    m_timeManager = TimeManager();
    
    // Main loop
    while (m_running) {
        runFrame();
    }
}

void GameLoop::stop() {
    m_running = false;
}

void GameLoop::runFrame() {
    // Update time values
    m_timeManager.update();
    
    // Variable update (input, game logic)
    if (m_updateCallback) {
        m_updateCallback(m_timeManager.getDeltaTime());
    }
    
    // Fixed update (physics)
    if (m_fixedUpdateCallback) {
        while (m_timeManager.shouldTakeFixedStep()) {
            m_fixedUpdateCallback(m_timeManager.getFixedTimeStep());
            m_timeManager.consumeFixedStep();
        }
    }
    
    // Render
    if (m_renderCallback) {
        m_renderCallback();
    }
}

} // namespace core
} // namespace engine
