#pragma once 

#include "Vector.h"
#include "Rotator.h"
#include "Quat.h"
#include "Matrix4.h"
struct Transform
{
public:
    RQuat Rotation;
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
    // const VectorRegister InputV = VectorLoadFloat3_W0(&V);

    // const VectorRegister RotatedVec = VectorQuaternionRotateVector(Rotation, InputV);

    // Vector Res;
    // VectorStoreFloat3(RotatedVec, &Res);
    // return Res;

    return Rotation.RotateVector(V);
}

inline Matrix4 Transform::ToMatrixWithScale() const 
{
    // Matrix4 Out;
    // use eular angle
    // alpha = z, beta = y, gama = x
    // const float cosx = cosf(Rotation.Roll);
    // const float sinx = sinf(Rotation.Pitch);
    // const float cosy = cosf(Rotation.Pitch);
    // const float siny = sinf(Rotation.Pitch);
    // const float cosz = cosf(Rotation.Yaw);
    // const float sinz = sinf(Rotation.Yaw);

    // Out.Mat[0][0] = cosz * cosy * Scale.X;
    // Out.Mat[1][1] = (sinz * siny  + cosz * cosx) * Scale.Y;
    // Out.Mat[2][2] = cosy * cosx * Scale.Z;

    // Out.Mat[0][1] = cosz * siny * sinx - sinz * cosx;
    // Out.Mat[0][2] = cosz * siny * cosx + sinz * sinx;
    // Out.Mat[1][0] = sinz * cosy ;
    // Out.Mat[1][2] = sinz * siny * cosx - cosz * sinx;
    // Out.Mat[2][0] = -siny;
    // Out.Mat[2][1] = cosy * sinx;

    // Out.Mat[0][3] = Translation.X;
    // Out.Mat[1][3] = Translation.Y;
    // Out.Mat[2][3] = Translation.Z;

    // Out.Mat[3][0] = 0.f;
    // Out.Mat[3][1] = 0.f;
    // Out.Mat[3][2] = 0.f;
    // Out.Mat[3][3] = 1.f;

    // use quaternion 
    
    Matrix4 OutMatrix;

#if !(UE_BUILD_SHIPPING || UE_BUILD_TEST) && WITH_EDITORONLY_DATA
		// Make sure Rotation is normalized when we turn it into a matrix.
		check(IsRotationNormalized());
#endif
	OutMatrix.Mat[0][3] = Translation.X;
	OutMatrix.Mat[1][3] = Translation.Y;
	OutMatrix.Mat[2][3] = Translation.Z;

	const float x2 = Rotation.X + Rotation.X;
	const float y2 = Rotation.Y + Rotation.Y;
	const float z2 = Rotation.Z + Rotation.Z;
	{
		const float xx2 = Rotation.X * x2;
		const float yy2 = Rotation.Y * y2;
		const float zz2 = Rotation.Z * z2;

		OutMatrix.Mat[0][0] = (1.0f - (yy2 + zz2)) * Scale.X;
		OutMatrix.Mat[1][1] = (1.0f - (xx2 + zz2)) * Scale.Y;
		OutMatrix.Mat[2][2] = (1.0f - (xx2 + yy2)) * Scale.Z;
	}
	{
		const float yz2 = Rotation.Y * z2;
		const float wx2 = Rotation.W * x2;

		OutMatrix.Mat[1][2] = (yz2 - wx2) * Scale.Z;
		OutMatrix.Mat[2][1] = (yz2 + wx2) * Scale.Y;
	}
	{
		const float xy2 = Rotation.X * y2;
		const float wz2 = Rotation.W * z2;

		OutMatrix.Mat[0][1] = (xy2 - wz2) * Scale.Y;
		OutMatrix.Mat[1][0] = (xy2 + wz2) * Scale.X;
	}
	{
		const float xz2 = Rotation.X * z2;
		const float wy2 = Rotation.W * y2;

		OutMatrix.Mat[0][2] = (xz2 + wy2) * Scale.Z;
		OutMatrix.Mat[2][0] = (xz2 - wy2) * Scale.X;
	}

	OutMatrix.Mat[3][0] = 0.0f;
	OutMatrix.Mat[3][1] = 0.0f;
	OutMatrix.Mat[3][2] = 0.0f;
	OutMatrix.Mat[3][3] = 1.0f;

	return OutMatrix;
}
