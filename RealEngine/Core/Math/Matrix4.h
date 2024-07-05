#pragma once 
#include "Vector.h"
#include "Vector4D.h"
#include "RealSSE.h"
#include "MathUtilityDefine.h"

struct Matrix4
{
    static const Matrix4 Identity ;

    Matrix4() {};
    Matrix4(float In);
    Matrix4(EIdentityTag); 
    Matrix4(const DirectX::XMMATRIX& InMat) { m_mat = InMat; };

    Matrix4(const Vector& InX, const Vector& InY, const Vector& InZ, const Vector& InW);
    Matrix4(const RVector4D& InX, const RVector4D& InY, const RVector4D& InZ, const RVector4D& InW);

    
    void SetIdentity();

    void SetAxis(uint8_t i,const RVector4D& Axis); 
    Vector3D GetAxis(uint8_t i)const;
    Matrix4 operator*(const Matrix4& M) const;
    void operator*=(const Matrix4& M);
    inline operator DirectX::XMMATRIX()const { return m_mat; }
    std::string ToString()const;
private:
    DirectX::XMMATRIX m_mat;  
};

inline Matrix4::Matrix4(EIdentityTag)
{
    m_mat = DirectX::XMMatrixIdentity();
}

inline Matrix4::Matrix4(const Vector& InX, const Vector& InY, const Vector& InZ, const Vector& InW)
{
    m_mat.r[0] = InX.GetWithWZero();
    m_mat.r[1] = InY.GetWithWZero();
    m_mat.r[2] = InZ.GetWithWZero();
    m_mat.r[3] = InW.GetWithWZero();
}
inline Matrix4::Matrix4(const RVector4D& InX, const RVector4D& InY, const RVector4D& InZ, const RVector4D& InW)
{
    m_mat.r[0] = InX;
    m_mat.r[1] = InY;
    m_mat.r[2] = InZ;
    m_mat.r[3] = InW;
}

inline void Matrix4::SetIdentity()
{
    *this = Matrix4::Identity;
};

inline void Matrix4::SetAxis(uint8_t i, const RVector4D& Axis)
{
    m_mat.r[i] = Axis;
}

inline Vector3D Matrix4::GetAxis(uint8_t i)const
{
    return Vector3D(m_mat.r[i]); 
}

inline Matrix4 Matrix4::operator*(const Matrix4& M) const 
{
    return Matrix4(DirectX::XMMatrixMultiply(M, m_mat)); 
};

inline void Matrix4::operator*=(const Matrix4& M)
{
    *this = DirectX::XMMatrixMultiply(M, m_mat);
};

inline std::string Matrix4::ToString()const
{
    return std::format("[{},\n{},\n{},\n{}]",  
        RVector4D(m_mat.r[0]).ToString(),  
        RVector4D(m_mat.r[1]).ToString(), 
        RVector4D(m_mat.r[2]).ToString(), 
        RVector4D(m_mat.r[3]).ToString()
        );
}
