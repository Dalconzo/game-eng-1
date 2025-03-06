#pragma once
#include "Vector.h"
#include "Matrix4x4.h"
#include <cmath>

namespace Engine {
namespace Math {

class Quaternion {
public:
    float x, y, z, w;
    
    Quaternion() : x(0), y(0), z(0), w(1) {}
    Quaternion(float x, float y, float z, float w) : x(x), y(y), z(z), w(w) {}
    
    // Create from euler angles (in radians)
    static Quaternion fromEulerAngles(float pitch, float yaw, float roll) {
        // Convert Euler angles to quaternion
        float cy = std::cos(yaw * 0.5f);
        float sy = std::sin(yaw * 0.5f);
        float cp = std::cos(pitch * 0.5f);
        float sp = std::sin(pitch * 0.5f);
        float cr = std::cos(roll * 0.5f);
        float sr = std::sin(roll * 0.5f);

        Quaternion q;
        q.w = cy * cp * cr + sy * sp * sr;
        q.x = cy * cp * sr - sy * sp * cr;
        q.y = sy * cp * sr + cy * sp * cr;
        q.z = sy * cp * cr - cy * sp * sr;
        
        return q;
    }

    Vector3 rotateVector(const Vector3& v) const {
        // Optimized implementation of v' = q * v * q^(-1)
        // This is the standard formula for rotating a vector by a quaternion
        Vector3 qvec(x, y, z);
        Vector3 uv = qvec.cross(v);
        Vector3 uuv = qvec.cross(uv);
        
        // v + 2.0 * (q.w * uv + uuv)
        return v + (uv * w + uuv) * 2.0f;
    }

    // Utility functions
    Quaternion normalized() const {
        float mag = std::sqrt(x*x + y*y + z*z + w*w);
        if (mag > 0) {
            return Quaternion(x/mag, y/mag, z/mag, w/mag);
        }
        return *this;
    }
    
    // Quaternion multiplication
    Quaternion operator*(const Quaternion& q) const {
        return Quaternion(
            w * q.x + x * q.w + y * q.z - z * q.y,
            w * q.y - x * q.z + y * q.w + z * q.x,
            w * q.z + x * q.y - y * q.x + z * q.w,
            w * q.w - x * q.x - y * q.y - z * q.z
        );
    }
    
    // Convert to euler angles
    Vector3 toEulerAngles() const {
        // Roll (x-axis rotation)
        float sinr_cosp = 2 * (w * x + y * z);
        float cosr_cosp = 1 - 2 * (x * x + y * y);
        float roll = std::atan2(sinr_cosp, cosr_cosp);

        // Pitch (y-axis rotation)
        float sinp = 2 * (w * y - z * x);
        float pitch;
        if (std::abs(sinp) >= 1)
            pitch = std::copysign(M_PI / 2, sinp); // Use 90 degrees if out of range
        else
            pitch = std::asin(sinp);

        // Yaw (z-axis rotation)
        float siny_cosp = 2 * (w * z + x * y);
        float cosy_cosp = 1 - 2 * (y * y + z * z);
        float yaw = std::atan2(siny_cosp, cosy_cosp);

        return Vector3(pitch, yaw, roll);
    }
    
    static Quaternion identity() {
        return Quaternion(0, 0, 0, 1);
    }

    static Quaternion fromMatrix(const Matrix4x4& m) {
        // Algorithm to convert a rotation matrix to a quaternion
        float trace = m(0, 0) + m(1, 1) + m(2, 2);
        Quaternion q;
        
        if (trace > 0) {
            float s = 0.5f / std::sqrt(trace + 1.0f);
            q.w = 0.25f / s;
            q.x = (m(2, 1) - m(1, 2)) * s;
            q.y = (m(0, 2) - m(2, 0)) * s;
            q.z = (m(1, 0) - m(0, 1)) * s;
        } else {
            if (m(0, 0) > m(1, 1) && m(0, 0) > m(2, 2)) {
                float s = 2.0f * std::sqrt(1.0f + m(0, 0) - m(1, 1) - m(2, 2));
                q.w = (m(2, 1) - m(1, 2)) / s;
                q.x = 0.25f * s;
                q.y = (m(0, 1) + m(1, 0)) / s;
                q.z = (m(0, 2) + m(2, 0)) / s;
            } else if (m(1, 1) > m(2, 2)) {
                float s = 2.0f * std::sqrt(1.0f + m(1, 1) - m(0, 0) - m(2, 2));
                q.w = (m(0, 2) - m(2, 0)) / s;
                q.x = (m(0, 1) + m(1, 0)) / s;
                q.y = 0.25f * s;
                q.z = (m(1, 2) + m(2, 1)) / s;
            } else {
                float s = 2.0f * std::sqrt(1.0f + m(2, 2) - m(0, 0) - m(1, 1));
                q.w = (m(1, 0) - m(0, 1)) / s;
                q.x = (m(0, 2) + m(2, 0)) / s;
                q.y = (m(1, 2) + m(2, 1)) / s;
                q.z = 0.25f * s;
            }
        }
        
        return q.normalized();
    }
};


} // namespace Math
} // namespace Engine
