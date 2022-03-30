#include "MathUtility.h"

// #include <cmath>

#include "Vector.h"
#include "Rotator.h"
#include "Transform.h"
#include "Matrix4.h"
#include "Vector2D.h"
#include "Quat.h"
#define FLOAT_NON_FRACTIONAL (8388608.f) /* All single-precision floating point numbers greater than or equal to this have no fractional value. */

const Vector Vector::ZeroVector(0.f, 0.f,0.f);
const Vector Vector::OneVector(1.f, 1.f, 1.f);
const Vector Vector::XAxisVector(1.0f, 0.0f, 0.0f);

const Rotator Rotator::ZeroRotator(0.f, 0.f,0.f);

RQuat Rotator::Quaternion() const 
{
		// DiagnosticCheckNaN();

#if PLATFORM_ENABLE_VECTORINTRINSICS
	const VectorRegister Angles = MakeVectorRegister(Pitch, Yaw, Roll, 0.0f);
	const VectorRegister AnglesNoWinding = VectorMod(Angles, GlobalVectorConstants::Float360);
	const VectorRegister HalfAngles = VectorMultiply(AnglesNoWinding, GlobalVectorConstants::DEG_TO_RAD_HALF);

	VectorRegister SinAngles, CosAngles;
	VectorSinCos(&SinAngles, &CosAngles, &HalfAngles);

	// Vectorized conversion, measured 20% faster than using scalar version after VectorSinCos.
	// Indices within VectorRegister (for shuffles): P=0, Y=1, R=2
	const VectorRegister SR = VectorReplicate(SinAngles, 2);
	const VectorRegister CR = VectorReplicate(CosAngles, 2);

	const VectorRegister SY_SY_CY_CY_Temp = VectorShuffle(SinAngles, CosAngles, 1, 1, 1, 1);

	const VectorRegister SP_SP_CP_CP = VectorShuffle(SinAngles, CosAngles, 0, 0, 0, 0);
	const VectorRegister SY_CY_SY_CY = VectorShuffle(SY_SY_CY_CY_Temp, SY_SY_CY_CY_Temp, 0, 2, 0, 2);

	const VectorRegister CP_CP_SP_SP = VectorShuffle(CosAngles, SinAngles, 0, 0, 0, 0);
	const VectorRegister CY_SY_CY_SY = VectorShuffle(SY_SY_CY_CY_Temp, SY_SY_CY_CY_Temp, 2, 0, 2, 0);

	const uint32 Neg = uint32(1 << 31);
	const uint32 Pos = uint32(0);
	const VectorRegister SignBitsLeft  = MakeVectorRegister(Pos, Neg, Pos, Pos);
	const VectorRegister SignBitsRight = MakeVectorRegister(Neg, Neg, Neg, Pos);
	const VectorRegister LeftTerm  = VectorBitwiseXor(SignBitsLeft , VectorMultiply(CR, VectorMultiply(SP_SP_CP_CP, SY_CY_SY_CY)));
	const VectorRegister RightTerm = VectorBitwiseXor(SignBitsRight, VectorMultiply(SR, VectorMultiply(CP_CP_SP_SP, CY_SY_CY_SY)));

	RQuat RotationQuat;
	const VectorRegister Result = VectorAdd(LeftTerm, RightTerm);	
	VectorStoreAligned(Result, &RotationQuat);
#else
	const float DEG_TO_RAD = PI/(180.f);
	const float RADS_DIVIDED_BY_2 = DEG_TO_RAD/2.f;
	float SP, SY, SR;
	float CP, CY, CR;

	const float PitchNoWinding = RMath::Fmod(Pitch, 360.0f);
	const float YawNoWinding = RMath::Fmod(Yaw, 360.0f);
	const float RollNoWinding = RMath::Fmod(Roll, 360.0f);

	RMath::SinCos(&SP, &CP, PitchNoWinding * RADS_DIVIDED_BY_2);
	RMath::SinCos(&SY, &CY, YawNoWinding * RADS_DIVIDED_BY_2);
	RMath::SinCos(&SR, &CR, RollNoWinding * RADS_DIVIDED_BY_2);

	RQuat RotationQuat;
	RotationQuat.X =  CR*SP*SY - SR*CP*CY;
	RotationQuat.Y = -CR*SP*CY - SR*CP*SY;
	RotationQuat.Z =  CR*CP*SY - SR*SP*CY;
	RotationQuat.W =  CR*CP*CY + SR*SP*SY;
#endif // PLATFORM_ENABLE_VECTORINTRINSICS

#if ENABLE_NAN_DIAGNOSTIC || DO_CHECK
	// Very large inputs can cause NaN's. Want to catch this here
	if (RotationQuat.ContainsNaN())
	{
		logOrEnsureNanError(TEXT("Invalid input %s to FRotator::Quaternion - generated NaN output: %s"), *ToString(), *RotationQuat.ToString());
		RotationQuat = RQuat::Identity;
	}
#endif

	return RotationQuat;
}


const Matrix4 Matrix4::Identity(Vector(1.f, 0.f,0.f), Vector(0.f, 1.f,0.f), Vector(0.f, 0.f,1.f), Vector(0.f));

float RMath::Radians(float Degrees)
{
	return Degrees * 0.01745329251994329576923690768489;
}

float RMath::Degrees(float Radians)
{
	return Radians * 57.295779513082320876798154814105;
}

float RMath::Clamp(float a, float min, float max)
{
	return a > max ? max : (a < min ? min : a);
}
Vector RMath::Cross(const Vector& v1, const Vector& v2)
{
	return Vector(v1.Y * v2.Z - v2.Y * v1.Z,
		            v1.Z * v2.X - v1.X * v2.Z,
		            v1.X * v2.Y - v1.Y * v2.X);
}
float RMath::Dot(const Vector& v1, const Vector& v2)
{
	return v1.X * v2.X + v1.Y * v2.Y + v1.Z * v2.Z;
}

Vector RMath::Normalize(const Vector& v)
{
	float Length=sqrtf( v.X * v.X + v.Y * v.Y + v.Z * v.Z);
	return v / Length;
}
float RMath::Fmod(float X, float Y)
{
	const float AbsY = fabsf(Y);
	if (AbsY <= 1.e-8f)
	{
		// FmodReportError(X, Y);
		return 0.f;
	}
	const float Div = (X / Y);
	// All floats where abs(f) >= 2^23 (8388608) are whole numbers so do not need truncation, and avoid overflow in TruncToFloat as they get even larger.
	const float Quotient = fabsf(Div) < FLOAT_NON_FRACTIONAL ? truncf(Div) : Div;
	float IntPortion = Y * Quotient;

	// Rounding and imprecision could cause IntPortion to exceed X and cause the result to be outside the expected range.
	// For example Fmod(55.8, 9.3) would result in a very small negative value!
	if (fabsf(IntPortion) > fabsf(X))
	{
		IntPortion = X;
	}

	const float Result = X - IntPortion;
	// Clamp to [-AbsY, AbsY] because of possible failures for very large numbers (>1e10) due to precision loss.
	// We could instead fall back to stock fmodf() for large values, however this would diverge from the SIMD VectorMod() which has no similar fallback with reasonable performance.
	return RMath::Clamp(Result, -AbsY, AbsY);
}
