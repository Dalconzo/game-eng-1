#pragma once
#include <cmath>

namespace Engine {
namespace Math {

struct Vector3 {
    float x, y, z;
    
    Vector3() : x(0), y(0), z(0) {}
    Vector3(float x, float y, float z) : x(x), y(y), z(z) {}
    
    // Basic vector operations
    Vector3 operator+(const Vector3& v) const { return Vector3(x + v.x, y + v.y, z + v.z); }
    Vector3 operator-(const Vector3& v) const { return Vector3(x - v.x, y - v.y, z - v.z); }
    Vector3 operator*(float scalar) const { return Vector3(x * scalar, y * scalar, z * scalar); }
    
    // Utility functions
    float magnitude() const { return std::sqrt(x*x + y*y + z*z); }
    
    Vector3 normalized() const {
        float mag = magnitude();
        if (mag > 0) {
            return Vector3(x / mag, y / mag, z / mag);
        }
        return *this;
    }
    
    static Vector3 zero() { return Vector3(0, 0, 0); }
    static Vector3 one() { return Vector3(1, 1, 1); }
    static Vector3 up() { return Vector3(0, 1, 0); }
    static Vector3 right() { return Vector3(1, 0, 0); }
    static Vector3 forward() { return Vector3(0, 0, 1); }

    Vector3 cross(const Vector3& v) const {
        return Vector3(
            y * v.z - z * v.y,
            z * v.x - x * v.z,
            x * v.y - y * v.x
        );
    }

    // Add this to the Vector3 struct
    float dot(const Vector3& other) const {
        return x * other.x + y * other.y + z * other.z;
    }

};

} // namespace Math
} // namespace Engine
