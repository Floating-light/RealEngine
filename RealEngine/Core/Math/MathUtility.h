#pragma once 
#include "Vector.h"
class RMath
{
public:
	// static Vector3D Cross(const Vector3D& v1, const Vector3D& v2);
	// static float Dot(const Vector3D& v1, const Vector3D& v2);
	// static Vector3D Normalize(const Vector3D& v);
	static float Radians(float Degrees);
	static float Degrees(float Radians);
	static float Clamp(float a, float min, float max);
    template<class T>
    static T Square(const T A)
    {
        return A*A;
    }
    template <class T> 
    static T Max(const T A, const T B )
    {
        return A >= B ? A : B;
    }
        template <class T> 
    static T Min(const T A, const T B )
    {
        return A <= B ? A : B;
    }
    template <class T> 
    static T Abs(const T A)
    {
        return A >= (T)0 ? A : -A;
    }

    static float InvSqrt(float In)
    {
        return 1.0f / std::sqrtf(In);
    }
    static float Sqrt(float In)
    {
        return std::sqrtf(In);
    }
    static int  IsFinite(float In)
    {
        return _finite(In);
    }

};