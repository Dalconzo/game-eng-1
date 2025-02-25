#pragma once

#include <chrono>

namespace engine {
namespace core {

class TimeManager {
public:
    TimeManager(double fixedTimeStep = 1.0/60.0);
    
    // Update time values - call once per frame
    void update();
    
    // Time getters
    double getDeltaTime() const { return m_deltaTime; }
    double getFixedTimeStep() const { return m_fixedTimeStep; }
    double getTotalTime() const { return m_totalTime; }
    unsigned int getFPS() const { return m_fps; }
    
    // Fixed timestep accumulator functions
    bool shouldTakeFixedStep();
    void consumeFixedStep();
    
private:
    using Clock = std::chrono::high_resolution_clock;
    using TimePoint = std::chrono::time_point<Clock>;
    
    TimePoint m_lastFrameTime;
    double m_deltaTime;      // Time between frames in seconds
    double m_totalTime;      // Total time since engine start
    double m_fixedTimeStep;  // Fixed physics timestep
    double m_accumulator;    // Accumulates time for fixed updates
    
    // FPS calculation
    unsigned int m_frameCount;
    unsigned int m_fps;
    double m_fpsTimer;
};

} // namespace core
} // namespace engine
