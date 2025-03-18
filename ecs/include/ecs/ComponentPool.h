// ComponentPool.h
#ifndef COMPONENT_POOL_H
#define COMPONENT_POOL_H

#include <vector>
#include <unordered_map>
#include "Entity.h"

// Base class for component pools.
class ComponentPoolBase {
public:
    virtual ~ComponentPoolBase() = default;
    virtual void remove(Entity entity) = 0; // Add this
    // Future: Add methods for removing components, checking existence, etc.
};

// Templated class for storing components of a specific type.
template <typename T>
class ComponentPool : public ComponentPoolBase {
public:
    T& add(Entity entity) {
        if (entity >= components.size()) {
            components.resize(entity + 1, {T{}, false});
        }
        components[entity].first = T{};
        components[entity].second = true; // Mark as active
        return components[entity].first;
    }

    T* get(Entity entity) {
        if (entity < components.size() && components[entity].second) {
            return &components[entity].first; // Return pointer if active
        }
        return nullptr;
    }

    void remove(Entity entity) {
        if (entity < components.size()) {
            components[entity].second = false; // Mark as inactive
        }
    }

    // Future: Add methods for removing components, iterating over components, etc.

private:
    std::unordered_map<Entity, size_t> entityToIndex; // Maps entities to their component indices.
    std::vector<std::pair<T, bool>> components; // Pair of component and active flag
};

#endif // COMPONENT_POOL_H