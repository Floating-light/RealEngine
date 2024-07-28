#pragma once 

#include <cassert>

#include "Vector.h"
#include "Rotator.h"
#include "Quat.h"
#include "Matrix4.h"
struct RTransform
{
public:
    RQuat Rotation;
    Vector Translation;
    Vector Scale;
 
    RTransform()
        : Rotation(kIdentity)
        , Translation(0.f)
        , Scale(Vector::OneVector)
        {}
    
    explicit RTransform(const Vector& InTranslation)
        : Rotation(kIdentity)
        , Translation(InTranslation)
        , Scale(Vector::OneVector)
        {}
    explicit RTransform(const Rotator& InRotation)
        : Rotation(InRotation.Quaternion()) 
        , Translation(Vector::ZeroVector)
        , Scale(Vector::OneVector)
        {}
    explicit RTransform(const RQuat& InRotation, const Vector& InTranslation, const Vector& InScale = Vector::OneVector)
        : Rotation(InRotation)
        , Translation(InTranslation)
        , Scale(InScale)
        {}
    inline RTransform operator~()const;
    void SetRatation(const RQuat& Quaternion) { Rotation = Quaternion; }
    void SetTranslation(const Vector& InVec) { Translation = InVec; }
    Vector TransformVectorNoScale(const Vector& V) const ;
    Matrix4 ToMatrixWithScale() const ;
    Matrix4 ToMatrixNoScale() const;
};

inline Vector RTransform::TransformVectorNoScale(const Vector& V) const 
{
    assert(0);
    // const VectorRegister InputV = VectorLoadFloat3_W0(&V);
    // const VectorRegister Rot = VectorLoadFloat3_W0(&Rotation);

    // const VectorRegister RotatedVec = VectorQuaternionRotateVector(Rot, InputV);

    // Vector Res;
    // VectorStoreFloat3(RotatedVec, &Res);
    // return Res;

    // return Rotation.RotateVector(V);
    return Vector();
}

inline Matrix4 RTransform::ToMatrixWithScale() const 
{
    Matrix4 RetVal = DirectX::XMMatrixRotationQuaternion(Rotation) * DirectX::XMMatrixScalingFromVector(Scale);
    RetVal.SetAxis(3, RVector4D(Translation, 1.f));
    return RetVal;
}

inline RTransform RTransform::operator~() const
{
    RQuat invertedQuat = ~Rotation;
    return RTransform(invertedQuat, invertedQuat * -Translation);
}

inline Matrix4 RTransform::ToMatrixNoScale() const
{
    Matrix4 RetVal = DirectX::XMMatrixRotationQuaternion(Rotation);
    RetVal.SetAxis(3, RVector4D(Translation, 1.f)); 
    return RetVal;
}

