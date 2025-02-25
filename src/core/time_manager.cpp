#include "core/time_manager.h"

namespace engine {
namespace core {

TimeManager::TimeManager(double fixedTimeStep)
    : m_lastFrameTime(Clock::now()),
      m_deltaTime(0.0),
      m_totalTime(0.0),
      m_fixedTimeStep(fixedTimeStep),
      m_accumulator(0.0),
      m_frameCount(0),
      m_fps(0),
      m_fpsTimer(0.0) {
}

void TimeManager::update() {
    // Calculate delta time
    TimePoint currentTime = Clock::now();
    std::chrono::duration<double> duration = currentTime - m_lastFrameTime;
    m_deltaTime = duration.count();
    
    // Cap deltaTime to avoid "spiral of death" with long frames
    if (m_deltaTime > 0.25) {
        m_deltaTime = 0.25;
    }
    
    m_lastFrameTime = currentTime;
    m_totalTime += m_deltaTime;
    
    // Accumulate time for fixed timestep updates
    m_accumulator += m_deltaTime;
    
    // FPS calculation
    m_frameCount++;
    m_fpsTimer += m_deltaTime;
    if (m_fpsTimer >= 1.0) {
        m_fps = m_frameCount;
        m_frameCount = 0;
        m_fpsTimer -= 1.0;
    }
}

bool TimeManager::shouldTakeFixedStep() {
    return m_accumulator >= m_fixedTimeStep;
}

void TimeManager::consumeFixedStep() {
    m_accumulator -= m_fixedTimeStep;
}

} // namespace core
} // namespace engine
