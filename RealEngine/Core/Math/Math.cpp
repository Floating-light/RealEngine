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
	return RQuat(Pitch, Yaw, Roll);
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
