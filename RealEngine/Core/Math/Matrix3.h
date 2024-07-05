#pragma once 

#include "Quat.h"
#include <DirectXMath.h>

// Represents a 3x3 matrix while occuping a 3x4 memory footprint.  The unused row and column are undefined but implicitly
// (0, 0, 0, 1).  Constructing a Matrix4 will make those values explicit.
__declspec(align(16)) class Matrix3
{
public:
    inline Matrix3() {}
    inline Matrix3(Vector x, Vector y, Vector z) { m_mat[0] = x; m_mat[1] = y; m_mat[2] = z; }
    //inline Matrix3(const Matrix3& m) { m_mat[0] = m.m_mat[0]; m_mat[1] = m.m_mat[1]; m_mat[2] = m.m_mat[2]; }
    inline Matrix3(RQuat q) { *this = Matrix3(DirectX::XMMatrixRotationQuaternion(q)); } 
    inline explicit Matrix3(const DirectX::XMMATRIX& m) { m_mat[0] = Vector(m.r[0]); m_mat[1] = Vector(m.r[1]); m_mat[2] = Vector(m.r[2]); }
    //inline explicit Matrix3(EIdentityTag) { m_mat[0] = Vector(kXUnitVector); m_mat[1] = Vector(kYUnitVector); m_mat[2] = Vector(kZUnitVector); }
    //inline explicit Matrix3(EZeroTag) { m_mat[0] = m_mat[1] = m_mat[2] = Vector(kZero); }

    inline void SetX(Vector x) { m_mat[0] = x; }
    inline void SetY(Vector y) { m_mat[1] = y; }
    inline void SetZ(Vector z) { m_mat[2] = z; }

    inline Vector GetX() const { return m_mat[0]; }
    inline Vector GetY() const { return m_mat[1]; }
    inline Vector GetZ() const { return m_mat[2]; }

    static inline Matrix3 MakeXRotation(float angle) { return Matrix3(DirectX::XMMatrixRotationX(angle)); }
    static inline Matrix3 MakeYRotation(float angle) { return Matrix3(DirectX::XMMatrixRotationY(angle)); }
    static inline Matrix3 MakeZRotation(float angle) { return Matrix3(DirectX::XMMatrixRotationZ(angle)); }
    //static inline Matrix3 MakeScale(float scale) { return Matrix3(XMMatrixScaling(scale, scale, scale)); }
    //static inline Matrix3 MakeScale(float sx, float sy, float sz) { return Matrix3(XMMatrixScaling(sx, sy, sz)); }
    //static inline Matrix3 MakeScale(const XMFLOAT3& scale) { return Matrix3(XMMatrixScaling(scale.x, scale.y, scale.z)); }
    //static inline Matrix3 MakeScale(Vector scale) { return Matrix3(XMMatrixScalingFromVector(scale)); }

    // Useful for DirectXMath interaction.  WARNING:  Only the 3x3 elements are defined.
    inline operator DirectX::XMMATRIX() const { return DirectX::XMMATRIX(m_mat[0], m_mat[1], m_mat[2], DirectX::XMVectorZero()); } 

    //inline Matrix3 operator* (Scalar scl) const { return Matrix3(scl * GetX(), scl * GetY(), scl * GetZ()); }
    inline Vector operator* (Vector vec) const { return Vector(DirectX::XMVector3TransformNormal(vec, *this)); } 
    inline Matrix3 operator* (const Matrix3& mat) const { return Matrix3(*this * mat.GetX(), *this * mat.GetY(), *this * mat.GetZ()); }

private:
    Vector m_mat[3]; 
};