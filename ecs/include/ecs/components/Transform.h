#ifndef TRANSFORM_H
#define TRANSFORM_H

struct Transform {
    float posX = 0.0f, posY = 0.0f, posZ = 0.0f;
    float rotX = 0.0f, rotY = 0.0f, rotZ = 0.0f;
    float scaleX = 1.0f, scaleY = 1.0f, scaleZ = 1.0f;
};

#endif // TRANSFORM_H