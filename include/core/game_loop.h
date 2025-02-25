#pragma once

#include "core/time_manager.h"
#include <functional>

namespace engine {
namespace core {

class GameLoop {
public:
    // Callback types for update and render
    using UpdateCallback = std::function<void(double)>;
    using FixedUpdateCallback = std::function<void(double)>;
    using RenderCallback = std::function<void()>;
    
    GameLoop();
    
    // Set callbacks
    void setUpdateCallback(UpdateCallback callback);
    void setFixedUpdateCallback(FixedUpdateCallback callback);
    void setRenderCallback(RenderCallback callback);
    
    // Main loop control
    void start();
    void stop();
    bool isRunning() const { return m_running; }
    
    // Time management
    TimeManager& getTimeManager() { return m_timeManager; }

        // Run a single frame of the game loop
    void runFrame();
    
private:
    bool m_running;
    TimeManager m_timeManager;
    
    UpdateCallback m_updateCallback;
    FixedUpdateCallback m_fixedUpdateCallback;
    RenderCallback m_renderCallback;
    

};

} // namespace core
} // namespace engine
