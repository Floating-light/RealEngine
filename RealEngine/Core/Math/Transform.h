#pragma once 

#include "Vector.h"
#include "Rotator.h"
#include "Matrix4.h"

struct Transform
{
public:
    Rotator Rotation;
    Vector Translation;
    Vector Scale;
 
    Transform()
        : Rotation(Rotator::ZeroRotator)
        , Translation(0.f)
        , Scale(Vector::OneVector)
        {}
    
    explicit Transform(const Vector& InTranslation)
        : Rotation(Rotator::ZeroRotator)
        , Translation(InTranslation)
        , Scale(Vector::OneVector)
        {}
    explicit Transform(const Rotator& InRotation)
        : Rotation(InRotation)
        , Translation(Vector::ZeroVector)
        , Scale(Vector::OneVector)
        {}
    explicit Transform(const Rotator& InRotation, const Vector& InTranslation, const Vector& InScale = Vector::OneVector)
        : Rotation(InRotation)
        , Translation(InTranslation)
        , Scale(InScale)
        {}

    Vector TransformVectorNoScale(const Vector& V) const ;
    Matrix4 ToMatrixWithScale() const ;
};

inline Vector Transform::TransformVectorNoScale(const Vector& V) const 
{
    const VectorRegister InputV = VectorLoadFloat3_W0(&V);

    const VectorRegister RotatedVec = VectorQuaternionRotateVector(Rotation, InputV);

    Vector Res;
    VectorStoreFloat3(RotatedVec, &Res);
    return Res;

    return Rotation.RotateVector(V);
}

inline Matrix4 Transform::ToMatrixWithScale() const 
{
    Matrix4 Out;
    // alpha = z, beta = y, gama = x
    const float cosx = cosf(Rotation.Roll);
    const float sinx = sinf(Rotation.Pitch);
    const float cosy = cosf(Rotation.Pitch);
    const float siny = sinf(Rotation.Pitch);
    const float cosz = cosf(Rotation.Yaw);
    const float sinz = sinf(Rotation.Yaw);

    Out.Mat[0][0] = cosz * cosy * Scale.X;
    Out.Mat[1][1] = (sinz * siny  + cosz * cosx) * Scale.Y;
    Out.Mat[2][2] = cosy * cosx * Scale.Z;

    Out.Mat[0][1] = cosz * siny * sinx - sinz * cosx;
    Out.Mat[0][2] = cosz * siny * cosx + sinz * sinx;
    Out.Mat[1][0] = sinz * cosy ;
    Out.Mat[1][2] = sinz * siny * cosx - cosz * sinx;
    Out.Mat[2][0] = -siny;
    Out.Mat[2][1] = cosy * sinx;

    Out.Mat[0][3] = Translation.X;
    Out.Mat[1][3] = Translation.Y;
    Out.Mat[2][3] = Translation.Z;

    Out.Mat[3][0] = 0.f;
    Out.Mat[3][1] = 0.f;
    Out.Mat[3][2] = 0.f;
    Out.Mat[3][3] = 1.f;

    return Out;
}
