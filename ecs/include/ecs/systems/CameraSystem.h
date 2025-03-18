#ifndef CAMERA_SYSTEM_H
#define CAMERA_SYSTEM_H

#include "../System.h"
#include "../ECSManager.h"
#include "../components/Transform.h"
#include "../components/Camera.h"

class CameraSystem : public System {
public:
    CameraSystem(Entity playerEntity) : playerEntity(playerEntity) {
        requiredComponents = {
            ECSManager::getComponentTypeId(typeid(Transform)),
            ECSManager::getComponentTypeId(typeid(Camera))
        };
    }

    void update(ECSManager* ecs) override {
        // Assume we have a way to identify the player entity (e.g., via a tag or ID)
        Entity player = ecs->getPlayerEntity();
        Transform* playerTransform = ecs->getComponent<Transform>(player);

        for (Entity entity : ecs->getEntitiesWithComponents(requiredComponents)) {
            Transform* camTransform = ecs->getComponent<Transform>(entity);
            if (playerTransform && camTransform) {
                // Camera follows player with an offset (e.g., centered behind)
                camTransform->posX = playerTransform->posX;
                camTransform->posY = playerTransform->posY + 2.0f;
                camTransform->posZ = playerTransform->posZ - 5.0f; // 5 units behind
            }
        }
    }
private:
    Entity playerEntity;
};

#endif // CAMERA_SYSTEM_H