#pragma once 
#include "Vector.h"
#include "Vector4D.h"
#include "RealSSE.h"

struct Matrix4
{
    float Mat[4][4];

    static const Matrix4 Identity ;

    Matrix4() ;
    Matrix4(float In);

    Matrix4(const Vector& InX, const Vector& InY, const Vector& InZ, const Vector& InW);
    Matrix4(const RVector4D& InX, const RVector4D& InY, const RVector4D& InZ, const RVector4D& InW);

    
    void SetIdentity();

    void SetAxis(uint8_t i,const Vector3D& Axis);

    Matrix4 operator*(const Matrix4& M) const;
    void operator*=(const Matrix4& M);

    bool operator==(const Matrix4& M)const;
    bool operator!=(const Matrix4& M)const;

};

inline Matrix4::Matrix4() : Mat{0.f}
{

};

inline Matrix4::Matrix4(const Vector& InX, const Vector& InY, const Vector& InZ, const Vector& InW)
{
    Mat[0][0] = InX.X; Mat[0][1] = InX.Y; Mat[0][2] = InX.Z; Mat[0][3] = 0.f;
    Mat[1][0] = InY.X; Mat[1][1] = InY.Y; Mat[1][2] = InY.Z; Mat[1][3] = 0.f;
    Mat[2][0] = InZ.X; Mat[2][1] = InZ.Y; Mat[2][2] = InZ.Z; Mat[2][3] = 0.f;
    Mat[3][0] = InW.X; Mat[3][1] = InW.Y; Mat[3][2] = InW.Z; Mat[3][3] = 1.f;
}
inline Matrix4::Matrix4(const RVector4D& InX, const RVector4D& InY, const RVector4D& InZ, const RVector4D& InW)
{
    Mat[0][0] = InX.X; Mat[0][1] = InX.Y; Mat[0][2] = InX.Z; Mat[0][3] = InX.W;
    Mat[1][0] = InY.X; Mat[1][1] = InY.Y; Mat[1][2] = InY.Z; Mat[1][3] = InY.W;
    Mat[2][0] = InZ.X; Mat[2][1] = InZ.Y; Mat[2][2] = InZ.Z; Mat[2][3] = InZ.W;
    Mat[3][0] = InW.X; Mat[3][1] = InW.Y; Mat[3][2] = InW.Z; Mat[3][3] = InW.W;
}

inline void Matrix4::SetIdentity()
{
    *this = Matrix4::Identity;
};
inline void Matrix4::SetAxis(uint8_t i,const Vector3D& Axis)
{
    Mat[i][0] = Axis.X;
    Mat[i][1] = Axis.Y;
    Mat[i][2] = Axis.Z;
}

inline Matrix4 Matrix4::operator*(const Matrix4& M) const 
{
    Matrix4 Res;
    
    RealSSE::MatrixMultiply(&Res, this, &M);
    return Res;
};

inline void Matrix4::operator*=(const Matrix4& M)
{
    RealSSE::MatrixMultiply(this, this, &M);
};

inline bool Matrix4::operator==(const Matrix4& M) const 
{
    for(int i = 0; i < 4; i++)
    {
        for(int j = 0; j < 4; j++)
        {
            if(Mat[i][j] != M.Mat[i][j])
            {
                return false;
            }
        }
    }

    return true;
};

inline bool Matrix4::operator!=(const Matrix4& M) const 
{
    return !(*this == M);
};


struct LookFromMatrix : public Matrix4
{
    LookFromMatrix(const Vector& EyeLocation, const Vector& LookDirection, const Vector& UpDirection);
};

struct LookAtMatrix : public LookFromMatrix
{
    LookAtMatrix(const Vector& EyeLocation, const Vector& LookAtPosition, const Vector& UpDirection)
        : LookFromMatrix(EyeLocation, LookAtPosition - EyeLocation, UpDirection)
    {}
};