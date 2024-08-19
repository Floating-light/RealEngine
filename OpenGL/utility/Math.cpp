#include "Math.h"

#include "glm/gtc/matrix_transform.hpp"
Vector3D RMath::Cross(const Vector3D& v1, const Vector3D& v2)
{
	return Vector3D(v1.y * v2.z - v2.y * v1.z,
		            v1.z * v2.x - v1.x * v2.z,
		            v1.x * v2.y - v1.y * v2.x);
}
float RMath::Dot(const Vector3D& v1, const Vector3D& v2)
{
	return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}

Vector3D RMath::Normalize(const Vector3D& v)
{
	float Length=sqrtf( v.x * v.x + v.y * v.y + v.z * v.z);
	return v / Length;
}

Matrix4 RMath::ViewTransform(const Vector3D& Eye, const Vector3D& Target, const Vector3D& Up)
{
	const Vector3D graze = RMath::Normalize(Target - Eye);
	const Vector3D s(RMath::Normalize(RMath::Cross(graze, Up)));
	const Vector3D u(RMath::Cross(s, graze));
	// 先旋转了
	// 再平移 ?
	Matrix4 Result(1.0f);
	Result[0][0] = s.x;
	Result[1][0] = s.y;
	Result[2][0] = s.z;
	Result[0][1] = u.x;
	Result[1][1] = u.y;
	Result[2][1] = u.z;
	Result[0][2] = -graze.x;
	Result[1][2] = -graze.y;
	Result[2][2] = -graze.z;

	Result[3][0] = -RMath::Dot(s, Eye);
	Result[3][1] = -RMath::Dot(u, Eye);
	Result[3][2] =  RMath::Dot(graze, Eye);

	return Result;
}

Matrix4 RMath::PerspectiveTransform(float fov, float aspect, float znear, float zfar)
{
	//float const tanHalfFovy = tan(fov / 2.0f);
	//Matrix4 Result(0.0f);
	//Result[0][0] = 1.0f / (aspect * tanHalfFovy);
	//Result[1][1] = 1.0f / (tanHalfFovy);
	//Result[2][2] = -(zfar + znear) / (zfar - znear);
	//Result[2][3] = -1.0f;
	//Result[3][2] = -(2.0f * zfar * znear) / (zfar - znear);
	return glm::perspective(fov, aspect, znear, zfar);
}


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
