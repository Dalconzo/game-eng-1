#ifndef CAMERA_H
#define CAMERA_H

struct Camera {
    float fov = 45.0f;
    float aspect = 800.0f / 600.0f;
    float nearPlane = 0.1f;
    float farPlane = 100.0f;
};

#endif // CAMERA_H