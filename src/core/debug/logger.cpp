#include "core/debug/logger.h"

namespace engine {
namespace core {
namespace debug {

Logger& Logger::getInstance() {
    static Logger instance;
    return instance;
}

Logger::Logger() : m_logLevel(LogLevel::INFO) {
    // Initialize logger
}

Logger::~Logger() {
    // Clean up logger
}

void Logger::info(const std::string& message, const std::string& file, int line) {
    log(LogLevel::INFO, message, file, line);
}

void Logger::debug(const std::string& message, const std::string& file, int line) {
    log(LogLevel::DEBUG, message, file, line);
}

void Logger::fatal(const std::string& message, const std::string& file, int line) {
    log(LogLevel::FATAL, message, file, line);
}

void Logger::log(LogLevel level, const std::string& message, const std::string& file, int line) {
    std::cout << "[" << getLevelString(level) << "] " << message << std::endl;
}

std::string Logger::getLevelString(LogLevel level) {
    switch(level) {
        case LogLevel::TRACE: return "TRACE";
        case LogLevel::DEBUG: return "DEBUG";
        case LogLevel::INFO: return "INFO";
        case LogLevel::WARNING: return "WARNING";
        case LogLevel::ERROR: return "ERROR";
        case LogLevel::FATAL: return "FATAL";
        default: return "UNKNOWN";
    }
}

}}} // namespace engine::core::debug
