#pragma once
#include "Vector.h"
#include <array>
#include <cmath>
#include <glm/glm.hpp>

namespace Engine {
namespace Math {

// Forward declaration
class Quaternion;

class Matrix4x4 {
public:
    // Column-major storage (OpenGL style)
    std::array<float, 16> data;
    
    Matrix4x4();
    
    // Access elements (row, column)
    float& operator()(int row, int col);
    float operator()(int row, int col) const;
    
    // Matrix multiplication
    Matrix4x4 operator*(const Matrix4x4& m) const;
    
    // Create translation matrix
    static Matrix4x4 createTranslation(const Vector3& position);
    
    // Create scale matrix
    static Matrix4x4 createScale(const Vector3& scale);
    
    // Create rotation matrix from quaternion - declaration only
    static Matrix4x4 createRotation(const Quaternion& q);
    
    // Create transformation matrix - declaration only
    static Matrix4x4 createTRS(const Vector3& position, const Quaternion& rotation, const Vector3& scale);
    
    // Create identity matrix
    static Matrix4x4 identity();

    // Convert to GLM matrix
    glm::mat4 toGLM() const;

    // In the Matrix4x4 class declaration
    static Matrix4x4 createLookAt(const Vector3& eye, const Vector3& target, const Vector3& up);

};

} // namespace Math
} // namespace Engine
