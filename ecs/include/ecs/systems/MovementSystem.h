#ifndef MOVEMENT_SYSTEM_H
#define MOVEMENT_SYSTEM_H

#include "../ECSManager.h"
#include "../components/Transform.h" // Changed from Position.h
#include "../components/Velocity.h"
#include <iostream>

class MovementSystem : public System {
public:
    MovementSystem() {
        requiredComponents = {
            ECSManager::getComponentTypeId(typeid(Transform)),
            ECSManager::getComponentTypeId(typeid(Velocity))
        };
    }
    void update(ECSManager* ecs) override {
        auto entities = ecs->getEntitiesWithComponents(requiredComponents);
        for (Entity entity : entities) {
            Transform* transform = ecs->getComponent<Transform>(entity);
            Velocity* vel = ecs->getComponent<Velocity>(entity);
            if (transform && vel) {
                transform->posX += vel->dx;
                transform->posY += vel->dy;
                transform->posZ += vel->dz;
                //std::cout << "Updated entity " << entity << " to position ("
                //          << transform->posX << ", " << transform->posY << ", " << transform->posZ << ")" << std::endl;
            } else {
                std::cout << "Transform or Velocity component not available!" << std::endl;
            }
        }
    }
};

#endif // MOVEMENT_SYSTEM_H