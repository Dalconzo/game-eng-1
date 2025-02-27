#pragma once
#include "Vector.h"
#include "Quaternion.h"
#include <array>
#include <cmath>

namespace Engine {
namespace Math {

class Matrix4x4 {
public:
    // Column-major storage (OpenGL style)
    std::array<float, 16> data;
    
    Matrix4x4() {
        // Initialize to identity matrix
        for (int i = 0; i < 16; i++) {
            data[i] = 0.0f;
        }
        data[0] = data[5] = data[10] = data[15] = 1.0f;
    }
    
    // Access elements (row, column)
    float& operator()(int row, int col) {
        return data[col * 4 + row];
    }
    
    float operator()(int row, int col) const {
        return data[col * 4 + row];
    }
    
    // Matrix multiplication
    Matrix4x4 operator*(const Matrix4x4& m) const {
        Matrix4x4 result;
        
        for (int row = 0; row < 4; row++) {
            for (int col = 0; col < 4; col++) {
                result(row, col) = 0.0f;
                for (int i = 0; i < 4; i++) {
                    result(row, col) += (*this)(row, i) * m(i, col);
                }
            }
        }
        
        return result;
    }
    
    // Create translation matrix
    static Matrix4x4 createTranslation(const Vector3& position) {
        Matrix4x4 result;
        result(0, 3) = position.x;
        result(1, 3) = position.y;
        result(2, 3) = position.z;
        return result;
    }
    
    // Create scale matrix
    static Matrix4x4 createScale(const Vector3& scale) {
        Matrix4x4 result;
        result(0, 0) = scale.x;
        result(1, 1) = scale.y;
        result(2, 2) = scale.z;
        return result;
    }
    
    // Create rotation matrix from quaternion
    static Matrix4x4 createRotation(const Quaternion& q) {
        Matrix4x4 result;
        
        float xx = q.x * q.x;
        float xy = q.x * q.y;
        float xz = q.x * q.z;
        float xw = q.x * q.w;
        
        float yy = q.y * q.y;
        float yz = q.y * q.z;
        float yw = q.y * q.w;
        
        float zz = q.z * q.z;
        float zw = q.z * q.w;
        
        result(0, 0) = 1.0f - 2.0f * (yy + zz);
        result(0, 1) = 2.0f * (xy - zw);
        result(0, 2) = 2.0f * (xz + yw);
        
        result(1, 0) = 2.0f * (xy + zw);
        result(1, 1) = 1.0f - 2.0f * (xx + zz);
        result(1, 2) = 2.0f * (yz - xw);
        
        result(2, 0) = 2.0f * (xz - yw);
        result(2, 1) = 2.0f * (yz + xw);
        result(2, 2) = 1.0f - 2.0f * (xx + yy);
        
        return result;
    }
    
    // Create transformation matrix (TRS order: scale, then rotate, then translate)
    static Matrix4x4 createTRS(const Vector3& position, const Quaternion& rotation, const Vector3& scale) {
        Matrix4x4 t = createTranslation(position);
        Matrix4x4 r = createRotation(rotation);
        Matrix4x4 s = createScale(scale);
        
        return t * r * s;
    }
    
    // Create identity matrix
    static Matrix4x4 identity() {
        return Matrix4x4();
    }
};

} // namespace Math
} // namespace Engine
