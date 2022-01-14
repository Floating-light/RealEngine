#include "MathUtility.h"

// #include <cmath>

#include "Vector.h"
#include "Rotator.h"
#include "Transform.h"
#include "Matrix4.h"
#include "Vector2D.h"

const Vector Vector::ZeroVector(0.f, 0.f,0.f);
const Vector Vector::OneVector(1.f, 1.f, 1.f);

const Rotator Rotator::ZeroRotator(0.f, 0.f,0.f);

const Matrix4 Matrix4::Identity(Vector(1.f, 0.f,0.f), Vector(0.f, 1.f,0.f), Vector(0.f, 0.f,1.f), Vector(0.f));


// Vector3D RMath::Cross(const Vector3D& v1, const Vector3D& v2)
// {
// 	return Vector3D(v1.Y * v2.Z - v2.Y * v1.Z,
// 		            v1.Z * v2.X - v1.X * v2.Z,
// 		            v1.X * v2.Y - v1.Y * v2.Z);
// }
// float RMath::Dot(const Vector3D& v1, const Vector3D& v2)
// {
// 	return v1.X * v2.X + v1.Y * v2.Y + v1.Z * v2.Z;
// }

// Vector3D RMath::Normalize(const Vector3D& v)
// {
// 	float Length=sqrtf( v.Z * v.X + v.Y * v.Y + v.Z * v.Z);
// 	return v / Length;
// }

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
