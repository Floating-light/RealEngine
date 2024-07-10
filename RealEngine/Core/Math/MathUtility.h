#pragma once 
#include "MathUtilityDefine.h"
#include "Logging.h"
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
    static Vector3D Cross(const Vector3D& v1, const Vector3D& v2);
	static float Dot(const Vector3D& v1, const Vector3D& v2);
	static Vector3D Normalize(const Vector3D& v);

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
    /** 
	* Returns the floating-point remainder of X / Y
	* Warning: Always returns remainder toward 0, not toward the smaller multiple of Y.
	*			So for example Fmod(2.8f, 2) gives .8f as you would expect, however, Fmod(-2.8f, 2) gives -.8f, NOT 1.2f 
	*/
	static float Fmod(float X, float Y);

    /**
	* Computes the sine and cosine of a scalar value.
	*
	* @param ScalarSin	Pointer to where the Sin result should be stored
	* @param ScalarCos	Pointer to where the Cos result should be stored
	* @param Value  input angles 
	*/
	static inline void SinCos( float* ScalarSin, float* ScalarCos, float  Value )
	{
		// Map Value to y in [-pi,pi], x = 2*pi*quotient + remainder.
		float quotient = (INV_PI*0.5f)*Value;
		if (Value >= 0.0f)
		{
			quotient = (float)((int)(quotient + 0.5f));
		}
		else
		{
			quotient = (float)((int)(quotient - 0.5f));
		}
		float y = Value - (2.0f*PI)*quotient;

		// Map y to [-pi/2,pi/2] with sin(y) = sin(Value).
		float sign;
		if (y > HALF_PI)
		{
			y = PI - y;
			sign = -1.0f;
		}
		else if (y < -HALF_PI)
		{
			y = -PI - y;
			sign = -1.0f;
		}
		else
		{
			sign = +1.0f;
		}

		float y2 = y * y;

		// 11-degree minimax approximation
		*ScalarSin = ( ( ( ( (-2.3889859e-08f * y2 + 2.7525562e-06f) * y2 - 0.00019840874f ) * y2 + 0.0083333310f ) * y2 - 0.16666667f ) * y2 + 1.0f ) * y;

		// 10-degree minimax approximation
		float p = ( ( ( ( -2.6051615e-07f * y2 + 2.4760495e-05f ) * y2 - 0.0013888378f ) * y2 + 0.041666638f ) * y2 - 0.5f ) * y2 + 1.0f;
		*ScalarCos = sign*p;
	}
	static inline float Acos( float Value ) { return acosf( (Value<-1.f) ? -1.f : ((Value<1.f) ? Value : 1.f) ); }

	/**
	 * Returns value based on comparand. The main purpose of this function is to avoid
	 * branching based on floating point comparison which can be avoided via compiler
	 * intrinsics.
	 *
	 * Please note that we don't define what happens in the case of NaNs as there might
	 * be platform specific differences.
	 *
	 * @param	Comparand		Comparand the results are based on
	 * @param	ValueGEZero		Return value if Comparand >= 0
	 * @param	ValueLTZero		Return value if Comparand < 0
	 *
	 * @return	ValueGEZero if Comparand >= 0, ValueLTZero otherwise
	 */
	static constexpr inline float FloatSelect( float Comparand, float ValueGEZero, float ValueLTZero )
	{
		return Comparand >= 0.f ? ValueGEZero : ValueLTZero;
	}
	
	// 如果16位对齐，AlignMask应该是15，0xFF
	template <typename T> 
	static inline T AlignUpWithMask(T Value, size_t AlignMask) 
	{
		// 与~AlignMask按位&会使低位为0，达到对齐地效果
		return (T)(((size_t)Value + AlignMask) & (~AlignMask)); 
	}

	template <typename T>
	static inline T AlignUp(T Value, size_t Alignment)
	{
		return AlignUpWithMask(Value, Alignment - 1);
	}
};