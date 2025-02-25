#pragma once
#include <string>
#include <filesystem>

namespace engine {
namespace core {

class ResourceManager {
public:
    // Initialize with automatic project root detection
    static void init();
    
    // Initialize with explicit project root path
    // static void init(const std::string& projectRoot);
    
    // Resolve a relative path to an absolute path
    static std::string resolvePath(const std::string& relativePath);
    
private:
    static std::string s_projectRoot;
    
    // Attempts to find the project root by looking for common project markers
    static std::string findProjectRoot();
};

} // namespace core
} // namespace engine
