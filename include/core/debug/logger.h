#pragma once
#include <string>
#include <iostream>
#include <fstream>
#include <chrono>
#include <mutex>

namespace engine {
namespace core {
namespace debug {

enum class LogLevel {
    TRACE,
    DEBUG,
    INFO,
    WARNING,
    ERROR,
    FATAL
};

class Logger {
public:
    static Logger& getInstance();
    
    void setLogLevel(LogLevel level);
    void log(LogLevel level, const std::string& message, const std::string& file, int line);
    
    // Convenience methods
    void trace(const std::string& message, const std::string& file, int line);
    void debug(const std::string& message, const std::string& file, int line);
    void info(const std::string& message, const std::string& file, int line);
    void warning(const std::string& message, const std::string& file, int line);
    void error(const std::string& message, const std::string& file, int line);
    void fatal(const std::string& message, const std::string& file, int line);

private:
    Logger();
    ~Logger();
    
    LogLevel m_logLevel;
    std::mutex m_mutex;
    std::ofstream m_logFile;
    
    std::string getTimestamp();
    std::string getLevelString(LogLevel level);
};

// Macros for easy logging
#define LOG_TRACE(message) engine::core::debug::Logger::getInstance().trace(message, __FILE__, __LINE__)
#define LOG_DEBUG(message) engine::core::debug::Logger::getInstance().debug(message, __FILE__, __LINE__)
#define LOG_INFO(message) engine::core::debug::Logger::getInstance().info(message, __FILE__, __LINE__)
#define LOG_WARNING(message) engine::core::debug::Logger::getInstance().warning(message, __FILE__, __LINE__)
#define LOG_ERROR(message) engine::core::debug::Logger::getInstance().error(message, __FILE__, __LINE__)
#define LOG_FATAL(message) engine::core::debug::Logger::getInstance().fatal(message, __FILE__, __LINE__)

}}} // namespace engine::core::debug
