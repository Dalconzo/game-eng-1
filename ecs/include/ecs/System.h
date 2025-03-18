#ifndef SYSTEM_H
#define SYSTEM_H

#include <vector>
#include <cstddef>

class ECSManager;

class System {
public:
    virtual ~System() = default;
    virtual void update(ECSManager* ecs) = 0;
    std::vector<size_t> requiredComponents;
};

#endif // SYSTEM_H