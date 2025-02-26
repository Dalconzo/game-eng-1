#pragma once

#include <string>
#include <unordered_map>
#include <vector>
#include <memory>
#include <iostream>
#include <algorithm>

namespace engine {
namespace core {
namespace resources {

template <typename ResourceType>
class BaseResourceManager {
public:
    BaseResourceManager() = default;
    virtual ~BaseResourceManager() {
        clearAll();
    }
    
    // Get a resource, returns nullptr if not found
    std::shared_ptr<ResourceType> getResource(const std::string& id) {
        auto it = m_resourceCache.find(id);
        if (it != m_resourceCache.end()) {
            return it->second;
        }
        return nullptr;
    }
    
    // Add a resource to the cache
    void addResource(const std::string& id, std::shared_ptr<ResourceType> resource) {
        m_resourceCache[id] = resource;
        // Ensure we have metadata for this resource
        if (m_resourceMetadata.find(id) == m_resourceMetadata.end()) {
            m_resourceMetadata[id] = ResourceMetadata();
        }
    }
    
    // Unload a resource if it has no dependents
    bool unloadResource(const std::string& id) {
        // Check if resource exists
        auto it = m_resourceCache.find(id);
        if (it == m_resourceCache.end()) {
            return false;
        }
        
        // Check if resource has dependents
        if (hasDependents(id)) {
            std::cout << "Cannot unload resource '" << id 
                     << "' as it has dependents." << std::endl;
            return false;
        }
        
        // Remove dependencies
        auto& metadata = m_resourceMetadata[id];
        for (const auto& depId : metadata.dependencies) {
            removeDependentLink(depId, id);
        }
        
        // Remove the resource and its metadata
        m_resourceCache.erase(id);
        m_resourceMetadata.erase(id);
        
        std::cout << "Unloaded resource: " << id << std::endl;
        return true;
    }
    
    // Register a dependency between resources
    void addDependency(const std::string& resourceId, const std::string& dependencyId) {
        // Ensure both resources have metadata entries
        if (m_resourceMetadata.find(resourceId) == m_resourceMetadata.end()) {
            m_resourceMetadata[resourceId] = ResourceMetadata();
        }
        if (m_resourceMetadata.find(dependencyId) == m_resourceMetadata.end()) {
            m_resourceMetadata[dependencyId] = ResourceMetadata();
        }
        
        // Add dependency link (resourceId depends on dependencyId)
        auto& dependencies = m_resourceMetadata[resourceId].dependencies;
        if (std::find(dependencies.begin(), dependencies.end(), dependencyId) == dependencies.end()) {
            dependencies.push_back(dependencyId);
        }
        
        // Add dependent link (dependencyId is depended on by resourceId)
        auto& dependents = m_resourceMetadata[dependencyId].dependents;
        if (std::find(dependents.begin(), dependents.end(), resourceId) == dependents.end()) {
            dependents.push_back(resourceId);
        }
    }
    
    // Check if a resource has any dependents
    bool hasDependents(const std::string& id) const {
        auto it = m_resourceMetadata.find(id);
        if (it != m_resourceMetadata.end()) {
            return !it->second.dependents.empty();
        }
        return false;
    }
    
    // Get all resources that depend on this resource
    std::vector<std::string> getDependents(const std::string& id) const {
        auto it = m_resourceMetadata.find(id);
        if (it != m_resourceMetadata.end()) {
            return it->second.dependents;
        }
        return {};
    }
    
    // Get all resources this resource depends on
    std::vector<std::string> getDependencies(const std::string& id) const {
        auto it = m_resourceMetadata.find(id);
        if (it != m_resourceMetadata.end()) {
            return it->second.dependencies;
        }
        return {};
    }
    
    // Clear all resources (force=true to clear even with dependencies)
    void clearAll(bool force = false) {
        if (force) {
            size_t count = m_resourceCache.size();
            m_resourceCache.clear();
            m_resourceMetadata.clear();
            if (count > 0) {
                std::cout << "Forcibly cleared " << count << " resources" << std::endl;
            }
            return;
        }
        
        // Try to unload resources in an order that respects dependencies
        // This is a simple approach - a more sophisticated topological sort
        // would be better for complex dependency graphs
        bool progress = true;
        while (progress && !m_resourceCache.empty()) {
            progress = false;
            for (auto it = m_resourceCache.begin(); it != m_resourceCache.end();) {
                if (!hasDependents(it->first)) {
                    std::string id = it->first;
                    // Remove this resource's dependencies first
                    auto& deps = m_resourceMetadata[id].dependencies;
                    for (const auto& depId : deps) {
                        removeDependentLink(depId, id);
                    }
                    
                    // Then remove the resource itself
                    it = m_resourceCache.erase(it);
                    m_resourceMetadata.erase(id);
                    progress = true;
                } else {
                    ++it;
                }
            }
        }
        
        // If we still have resources, they have circular dependencies
        if (!m_resourceCache.empty()) {
            std::cout << "Warning: " << m_resourceCache.size() 
                     << " resources remain due to circular dependencies" << std::endl;
        }
    }
    
    // Statistics
    size_t getCacheSize() const { return m_resourceCache.size(); }
    
    // Print dependency information for debugging
    void printDependencyInfo() const {
        std::cout << "Resource Dependencies:" << std::endl;
        for (const auto& [id, metadata] : m_resourceMetadata) {
            std::cout << "Resource: " << id << std::endl;
            
            std::cout << "  Depends on: ";
            for (const auto& dep : metadata.dependencies) {
                std::cout << dep << " ";
            }
            std::cout << std::endl;
            
            std::cout << "  Depended on by: ";
            for (const auto& dep : metadata.dependents) {
                std::cout << dep << " ";
            }
            std::cout << std::endl;
        }
    }
    
protected:
    // Metadata for tracking resource dependencies
    struct ResourceMetadata {
        std::vector<std::string> dependencies;  // Resources this resource depends on
        std::vector<std::string> dependents;    // Resources that depend on this resource
    };
    
    // Resource cache and metadata
    std::unordered_map<std::string, std::shared_ptr<ResourceType>> m_resourceCache;
    std::unordered_map<std::string, ResourceMetadata> m_resourceMetadata;
    
private:
    // Remove a dependent link (helper method)
    void removeDependentLink(const std::string& resourceId, const std::string& dependentId) {
        auto it = m_resourceMetadata.find(resourceId);
        if (it != m_resourceMetadata.end()) {
            auto& dependents = it->second.dependents;
            auto depIt = std::find(dependents.begin(), dependents.end(), dependentId);
            if (depIt != dependents.end()) {
                dependents.erase(depIt);
            }
        }
    }
};

} // namespace resources
} // namespace core
} // namespace engine
