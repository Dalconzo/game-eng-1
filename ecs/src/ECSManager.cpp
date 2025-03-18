#include "../include/ecs/ECSManager.h"
#include <algorithm>

size_t ECSManager::getComponentTypeId(const std::type_info& ti) {
    static std::unordered_map<std::type_index, size_t> typeIds;
    static size_t nextId = 0;
    auto tiIndex = std::type_index(ti);
    auto it = typeIds.find(tiIndex);
    if (it == typeIds.end()) {
        typeIds[tiIndex] = nextId++;
    }
    return typeIds[tiIndex];
}

std::vector<Entity> ECSManager::getEntitiesWithComponents(const std::vector<size_t>& componentTypes) {
    std::vector<Entity> result;
    for (const auto& pair : entityComponents) {
        Entity entity = pair.first;
        const std::vector<size_t>& entityComps = pair.second;
        bool hasAll = true;
        for (size_t type : componentTypes) {
            if (std::find(entityComps.begin(), entityComps.end(), type) == entityComps.end()) {
                hasAll = false;
                break;
            }
        }
        if (hasAll) {
            result.push_back(entity);
        }
    }
    return result;
}