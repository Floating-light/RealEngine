#pragma once
#include <string>
#include <sstream>

#include "Vector.h"
#include "Math.h"
#include "Logging.h"

// #define FORCEINLINE inline
#define CORE_API
#define KINDA_SMALL_NUMBER	(1.e-4f)
#define SMALL_NUMBER		(1.e-8f)
/**
 * A vector in 2-D space composed of components (X, Y) with floating point precision.
 */
struct Vector2D 
{
	/** Vector's X component. */
	float X;

	/** Vector's Y component. */
	float Y;

public:

	/** Global 2D zero vector constant (0,0) */
	static CORE_API const Vector2D ZeroVector;

	/**
	* Global 2D one vector (poorly named) constant (1,1).
	*
	* @note Incorrectly named "unit" vector though its magnitude/length/size is not one. Would fix, though likely used all over the world. Use `Unit45Deg` below for an actual unit vector.
	*/
	static CORE_API const Vector2D UnitVector;

	/**
	* Global 2D unit vector constant along the 45 degree angle or symmetrical positive axes (sqrt(.5),sqrt(.5)) or (.707,.707). https://en.wikipedia.org/wiki/Unit_vector
	*
	* @note The `UnitVector` above is actually a value with axes of 1 rather than a magnitude of one.
	*/
	static CORE_API const Vector2D Unit45Deg;

public:

	/** Default constructor (no initialization). */
	FORCEINLINE Vector2D() { }

	/**
	 * Constructor using initial values for each component.
	 *
	 * @param InX X coordinate.
	 * @param InY Y coordinate.
	 */
	FORCEINLINE Vector2D(float InX, float InY);
	
	/**
	 * Constructor initializing both components to a single float value.
	 *
	 * @param InF Value to set both components to.
	 */
	explicit FORCEINLINE Vector2D(float InF);

	/**
	 * Constructs a vector from an FVector.
	 * Copies the X and Y components from the FVector.
	 *
	 * @param V Vector to copy from.
	 */
	explicit FORCEINLINE Vector2D(const Vector3D& V);

public:

	/**
	 * Gets the result of adding two vectors together.
	 *
	 * @param V The other vector to add to this.
	 * @return The result of adding the vectors together.
	 */
	FORCEINLINE Vector2D operator+(const Vector2D& V) const;

	/**
	 * Gets the result of subtracting a vector from this one.
	 *
	 * @param V The other vector to subtract from this.
	 * @return The result of the subtraction.
	 */
	FORCEINLINE Vector2D operator-(const Vector2D& V) const;

	/**
	 * Gets the result of scaling the vector (multiplying each component by a value).
	 *
	 * @param Scale How much to scale the vector by.
	 * @return The result of scaling this vector.
	 */
	FORCEINLINE Vector2D operator*(float Scale) const;

	/**
	 * Gets the result of dividing each component of the vector by a value.
	 *
	 * @param Scale How much to divide the vector by.
	 * @return The result of division on this vector.
	 */
	Vector2D operator/(float Scale) const;

	/**
	 * Gets the result of adding A to each component of the vector.
	 *
	 * @param A Float to add to each component.
	 * @return The result of adding A to each component.
	 */
	FORCEINLINE Vector2D operator+(float A) const;

	/**
	 * Gets the result of subtracting A from each component of the vector.
	 *
	 * @param A Float to subtract from each component
	 * @return The result of subtracting A from each component.
	 */
	FORCEINLINE Vector2D operator-(float A) const;

	/**
	 * Gets the result of component-wise multiplication of this vector by another.
	 *
	 * @param V The other vector to multiply this by.
	 * @return The result of the multiplication.
	 */
	FORCEINLINE Vector2D operator*(const Vector2D& V) const;

	/**
	 * Gets the result of component-wise division of this vector by another.
	 *
	 * @param V The other vector to divide this by.
	 * @return The result of the division.
	 */
	Vector2D operator/(const Vector2D& V) const;

	/**
	 * Calculates dot product of this vector and another.
	 *
	 * @param V The other vector.
	 * @return The dot product.
	 */
	FORCEINLINE float operator|(const Vector2D& V) const;

	/**
	 * Calculates cross product of this vector and another.
	 *
	 * @param V The other vector.
	 * @return The cross product.
	 */
	FORCEINLINE float operator^(const Vector2D& V) const;

public:

	/**
	 * Compares this vector against another for equality.
	 *
	 * @param V The vector to compare against.
	 * @return true if the two vectors are equal, otherwise false.
	 */
	bool operator==(const Vector2D& V) const;

	/**
	 * Compares this vector against another for inequality.
	 *
	 * @param V The vector to compare against.
	 * @return true if the two vectors are not equal, otherwise false.
	 */
	bool operator!=(const Vector2D& V) const;

	/**
	 * Checks whether both components of this vector are less than another.
	 *
	 * @param Other The vector to compare against.
	 * @return true if this is the smaller vector, otherwise false.
	 */
	bool operator<(const Vector2D& Other) const;

	/**
	 * Checks whether both components of this vector are greater than another.
	 *
	 * @param Other The vector to compare against.
	 * @return true if this is the larger vector, otherwise false.
	 */
	bool operator>(const Vector2D& Other) const;

	/**
	 * Checks whether both components of this vector are less than or equal to another.
	 *
	 * @param Other The vector to compare against.
	 * @return true if this vector is less than or equal to the other vector, otherwise false.
	 */
	bool operator<=(const Vector2D& Other) const;

	/**
	 * Checks whether both components of this vector are greater than or equal to another.
	 *
	 * @param Other The vector to compare against.
	 * @return true if this vector is greater than or equal to the other vector, otherwise false.
	 */
	bool operator>=(const Vector2D& Other) const;

	/**
	 * Gets a negated copy of the vector.
	 *
	 * @return A negated copy of the vector.
	 */
	FORCEINLINE Vector2D operator-() const;

	/**
	 * Adds another vector to this.
	 *
	 * @param V The other vector to add.
	 * @return Copy of the vector after addition.
	 */
	FORCEINLINE Vector2D operator+=(const Vector2D& V);

	/**
	 * Subtracts another vector from this.
	 *
	 * @param V The other vector to subtract.
	 * @return Copy of the vector after subtraction.
	 */
	FORCEINLINE Vector2D operator-=(const Vector2D& V);

	/**
	 * Scales this vector.
	 *
	 * @param Scale The scale to multiply vector by.
	 * @return Copy of the vector after scaling.
	 */
	FORCEINLINE Vector2D operator*=(float Scale);

	/**
	 * Divides this vector.
	 *
	 * @param V What to divide vector by.
	 * @return Copy of the vector after division.
	 */
	Vector2D operator/=(float V);

	/**
	 * Multiplies this vector with another vector, using component-wise multiplication.
	 *
	 * @param V The vector to multiply with.
	 * @return Copy of the vector after multiplication.
	 */
	Vector2D operator*=(const Vector2D& V);

	/**
	 * Divides this vector by another vector, using component-wise division.
	 *
	 * @param V The vector to divide by.
	 * @return Copy of the vector after division.
	 */
	Vector2D operator/=(const Vector2D& V);

	/**
	 * Gets specific component of the vector.
	 *
	 * @param Index the index of vector component
	 * @return reference to component.
	 */
	float& operator[](int Index);

	/**
	 * Gets specific component of the vector.
	 *
	 * @param Index the index of vector component
	 * @return copy of component value.
	 */
	float operator[](int Index) const;

	/**
	* Gets a specific component of the vector.
	*
	* @param Index The index of the component required.
	* @return Reference to the specified component.
	*/
	float& Component(int Index);

	/**
	* Gets a specific component of the vector.
	*
	* @param Index The index of the component required.
	* @return Copy of the specified component.
	*/
	float Component(int Index) const;

public:

	/**
	 * Calculates the dot product of two vectors.
	 *
	 * @param A The first vector.
	 * @param B The second vector.
	 * @return The dot product.
	 */
	FORCEINLINE static float DotProduct(const Vector2D& A, const Vector2D& B);

	/**
	 * Squared distance between two 2D points.
	 *
	 * @param V1 The first point.
	 * @param V2 The second point.
	 * @return The squared distance between two 2D points.
	 */
	FORCEINLINE static float DistSquared(const Vector2D& V1, const Vector2D& V2);

	/**
	 * Distance between two 2D points.
	 *
	 * @param V1 The first point.
	 * @param V2 The second point.
	 * @return The distance between two 2D points.
	 */
	FORCEINLINE static float Distance(const Vector2D& V1, const Vector2D& V2);

	/**
	 * Calculate the cross product of two vectors.
	 *
	 * @param A The first vector.
	 * @param B The second vector.
	 * @return The cross product.
	 */
	FORCEINLINE static float CrossProduct(const Vector2D& A, const Vector2D& B);

	/**
	 * Returns a vector with the maximum component for each dimension from the pair of vectors.
	 *
	 * @param A The first vector.
	 * @param B The second vector.
	 * @return The max vector.
	 */
	FORCEINLINE static Vector2D Max(const Vector2D& A, const Vector2D& B);

	/**
	 * Returns a vector with the minimum component for each dimension from the pair of vectors.
	 *
	 * @param A The first vector.
	 * @param B The second vector.
	 * @return The min vector.
	 */
	FORCEINLINE static Vector2D Min(const Vector2D& A, const Vector2D& B);

	/**
	 * Checks for equality with error-tolerant comparison.
	 *
	 * @param V The vector to compare.
	 * @param Tolerance Error tolerance.
	 * @return true if the vectors are equal within specified tolerance, otherwise false.
	 */
	bool Equals(const Vector2D& V, float Tolerance=KINDA_SMALL_NUMBER) const;

	/**
	 * Set the values of the vector directly.
	 *
	 * @param InX New X coordinate.
	 * @param InY New Y coordinate.
	 */
	void Set(float InX, float InY);

	/**
	 * Get the maximum value of the vector's components.
	 *
	 * @return The maximum value of the vector's components.
	 */
	float GetMax() const;

	/**
	 * Get the maximum absolute value of the vector's components.
	 *
	 * @return The maximum absolute value of the vector's components.
	 */
	float GetAbsMax() const;

	/**
	 * Get the minimum value of the vector's components.
	 *
	 * @return The minimum value of the vector's components.
	 */
	float GetMin() const;

	/**
	 * Get the length (magnitude) of this vector.
	 *
	 * @return The length of this vector.
	 */
	float Size() const;

	/**
	 * Get the squared length of this vector.
	 *
	 * @return The squared length of this vector.
	 */
	float SizeSquared() const;

	/**
	 * Rotates around axis (0,0,1)
	 *
	 * @param AngleDeg Angle to rotate (in degrees)
	 * @return Rotated Vector
	 */
	// Vector2D GetRotated(float AngleDeg) const;

	/**
	 * Gets a normalized copy of the vector, checking it is safe to do so based on the length.
	 * Returns zero vector if vector length is too small to safely normalize.
	 *
	 * @param Tolerance Minimum squared length of vector for normalization.
	 * @return A normalized copy of the vector if safe, (0,0) otherwise.
	 */
	// Vector2D GetSafeNormal(float Tolerance=SMALL_NUMBER) const;

	/**
	 * Normalize this vector in-place if it is large enough, set it to (0,0) otherwise.
	 *
	 * @param Tolerance Minimum squared length of vector for normalization.
	 * @see GetSafeNormal()
	 */
	// void Normalize(float Tolerance=SMALL_NUMBER);

	/**
	 * Checks whether vector is near to zero within a specified tolerance.
	 *
	 * @param Tolerance Error tolerance.
	 * @return true if vector is in tolerance to zero, otherwise false.
	 */
	bool IsNearlyZero(float Tolerance=KINDA_SMALL_NUMBER) const;

	/**
	 * Util to convert this vector into a unit direction vector and its original length.
	 *
	 * @param OutDir Reference passed in to store unit direction vector.
	 * @param OutLength Reference passed in to store length of the vector.
	 */
	void ToDirectionAndLength(Vector2D &OutDir, float &OutLength) const;

	/**
	 * Checks whether all components of the vector are exactly zero.
	 *
	 * @return true if vector is exactly zero, otherwise false.
	 */
	bool IsZero() const;

	/**
	 * Get this vector as a vector where each component has been rounded to the nearest int.
	 *
	 * @return New Vector2D from this vector that is rounded.
	 */
	// Vector2D RoundToVector() const;

	/**
	 * Creates a copy of this vector with both axes clamped to the given range.
	 * @return New vector with clamped axes.
	 */
	// Vector2D ClampAxes(float MinAxisVal, float MaxAxisVal) const;

	/**
	* Get a copy of the vector as sign only.
	* Each component is set to +1 or -1, with the sign of zero treated as +1.
	*
	* @param A copy of the vector with each component set to +1 or -1
	*/
	// FORCEINLINE Vector2D GetSignVector() const;

	/**
	* Get a copy of this vector with absolute value of each component.
	*
	* @return A copy of this vector with absolute value of each component.
	*/
	FORCEINLINE Vector2D GetAbs() const;

	/**
	 * Get a textual representation of the vector.
	 *
	 * @return Text describing the vector.
	 */
	std::string ToString() const;

	/**
	 * Utility to check if there are any non-finite values (NaN or Inf) in this vector.
	 *
	 * @return true if there are any non-finite values in this vector, false otherwise.
	 */
	// FORCEINLINE bool ContainsNaN() const
	// {
	// 	return (!FMath::IsFinite(X) || 
	// 			!FMath::IsFinite(Y));
	// }

	/**
	 * Network serialization function.
	 * FVectors NetSerialize without quantization (ie exact values are serialized).
	 */
	// CORE_API bool NetSerialize(FArchive& Ar, class UPackageMap* Map, bool& bOutSuccess);

	/** Converts spherical coordinates on the unit sphere into a Cartesian unit length vector. */
	// inline Vector3D SphericalToUnitCartesian() const;
};

/**
 * Creates a hash value from a Vector2D. 
 *
 * @param Vector the vector to create a hash value for
 * @return The hash value from the components
 */
// FORCEINLINE uint32 GetTypeHash(const Vector2D& Vector)
// {
// 	// Note: this assumes there's no padding in Vector2D that could contain uncompared data.
// 	return FCrc::MemCrc_DEPRECATED(&Vector,sizeof(Vector));
// }

/* Vector2D inline functions
 *****************************************************************************/

FORCEINLINE Vector2D operator*(float Scale, const Vector2D& V)
{
	return V.operator*(Scale);
}

// template <> struct TIsPODType<Vector2D> { enum { Value = true }; };


FORCEINLINE Vector2D::Vector2D(float InX,float InY)
	:	X(InX), Y(InY)
{ }

FORCEINLINE Vector2D::Vector2D(float InF)
	:	X(InF), Y(InF)
{ }

FORCEINLINE Vector2D Vector2D::operator+(const Vector2D& V) const
{
	return Vector2D(X + V.X, Y + V.Y);
}


FORCEINLINE Vector2D Vector2D::operator-(const Vector2D& V) const
{
	return Vector2D(X - V.X, Y - V.Y);
}


FORCEINLINE Vector2D Vector2D::operator*(float Scale) const
{
	return Vector2D(X * Scale, Y * Scale);
}


FORCEINLINE Vector2D Vector2D::operator/(float Scale) const
{
	const float RScale = 1.f/Scale;
	return Vector2D(X * RScale, Y * RScale);
}


FORCEINLINE Vector2D Vector2D::operator+(float A) const
{
	return Vector2D(X + A, Y + A);
}


FORCEINLINE Vector2D Vector2D::operator-(float A) const
{
	return Vector2D(X - A, Y - A);
}


FORCEINLINE Vector2D Vector2D::operator*(const Vector2D& V) const
{
	return Vector2D(X * V.X, Y * V.Y);
}


FORCEINLINE Vector2D Vector2D::operator/(const Vector2D& V) const
{
	return Vector2D(X / V.X, Y / V.Y);
}


FORCEINLINE float Vector2D::operator|(const Vector2D& V) const
{
	return X*V.X + Y*V.Y;
}


FORCEINLINE float Vector2D::operator^(const Vector2D& V) const
{
	return X*V.Y - Y*V.X;
}


FORCEINLINE float Vector2D::DotProduct(const Vector2D& A, const Vector2D& B)
{
	return A | B;
}


FORCEINLINE float Vector2D::DistSquared(const Vector2D &V1, const Vector2D &V2)
{
	return RMath::Square(V2.X-V1.X) + RMath::Square(V2.Y-V1.Y);
}


FORCEINLINE float Vector2D::Distance(const Vector2D& V1, const Vector2D& V2)
{
	return std::sqrtf(Vector2D::DistSquared(V1, V2));
}


FORCEINLINE float Vector2D::CrossProduct(const Vector2D& A, const Vector2D& B)
{
	return A ^ B;
}

FORCEINLINE Vector2D Vector2D::Max(const Vector2D& A, const Vector2D& B)
{
	return Vector2D(RMath::Max(A.X, B.X), RMath::Max(A.Y, B.Y));
}

FORCEINLINE Vector2D Vector2D::Min(const Vector2D& A, const Vector2D& B)
{
	return Vector2D(RMath::Min(A.X, B.X), RMath::Min(A.Y, B.Y));
}

FORCEINLINE bool Vector2D::operator==(const Vector2D& V) const
{
	return X==V.X && Y==V.Y;
}


FORCEINLINE bool Vector2D::operator!=(const Vector2D& V) const
{
	return X!=V.X || Y!=V.Y;
}


FORCEINLINE bool Vector2D::operator<(const Vector2D& Other) const
{
	return X < Other.X && Y < Other.Y;
}


FORCEINLINE bool Vector2D::operator>(const Vector2D& Other) const
{
	return X > Other.X && Y > Other.Y;
}


FORCEINLINE bool Vector2D::operator<=(const Vector2D& Other) const
{
	return X <= Other.X && Y <= Other.Y;
}


FORCEINLINE bool Vector2D::operator>=(const Vector2D& Other) const
{
	return X >= Other.X && Y >= Other.Y;
}


FORCEINLINE bool Vector2D::Equals(const Vector2D& V, float Tolerance) const
{
	return RMath::Abs(X-V.X) <= Tolerance && RMath::Abs(Y-V.Y) <= Tolerance;
}


FORCEINLINE Vector2D Vector2D::operator-() const
{
	return Vector2D(-X, -Y);
}


FORCEINLINE Vector2D Vector2D::operator+=(const Vector2D& V)
{
	X += V.X; Y += V.Y;
	return *this;
}


FORCEINLINE Vector2D Vector2D::operator-=(const Vector2D& V)
{
	X -= V.X; Y -= V.Y;
	return *this;
}


FORCEINLINE Vector2D Vector2D::operator*=(float Scale)
{
	X *= Scale; Y *= Scale;
	return *this;
}


FORCEINLINE Vector2D Vector2D::operator/=(float V)
{
	const float RV = 1.f/V;
	X *= RV; Y *= RV;
	return *this;
}


FORCEINLINE Vector2D Vector2D::operator*=(const Vector2D& V)
{
	X *= V.X; Y *= V.Y;
	return *this;
}


FORCEINLINE Vector2D Vector2D::operator/=(const Vector2D& V)
{
	X /= V.X; Y /= V.Y;
	return *this;
}


FORCEINLINE float& Vector2D::operator[](int Index)
{
	CHECK(Index>=0 && Index<2);
	return ((Index == 0) ? X : Y);
}


FORCEINLINE float Vector2D::operator[](int Index) const
{
	CHECK(Index>=0 && Index<2);
	return ((Index == 0) ? X : Y);
}


FORCEINLINE void Vector2D::Set(float InX, float InY)
{
	X = InX;
	Y = InY;
}


FORCEINLINE float Vector2D::GetMax() const
{
	return RMath::Max(X,Y);
}


FORCEINLINE float Vector2D::GetAbsMax() const
{
	return RMath::Max(RMath::Abs(X),RMath::Abs(Y));
}


FORCEINLINE float Vector2D::GetMin() const
{
	return RMath::Min(X,Y);
}


FORCEINLINE float Vector2D::Size() const
{
	return sqrtf(X*X + Y*Y);
}


FORCEINLINE float Vector2D::SizeSquared() const
{
	return X*X + Y*Y;
}


// FORCEINLINE Vector2D Vector2D::GetRotated(const float AngleDeg) const
// {
// 	// Based on FVector::RotateAngleAxis with Axis(0,0,1)

// 	float S, C;
// 	FMath::SinCos(&S, &C, FMath::DegreesToRadians(AngleDeg));

// 	const float OMC = 1.0f - C;

// 	return Vector2D(
// 		C * X - S * Y,
// 		S * X + C * Y);
// }


// FORCEINLINE Vector2D Vector2D::GetSafeNormal(float Tolerance) const
// {	
// 	const float SquareSum = X*X + Y*Y;
// 	if(SquareSum > Tolerance)
// 	{
// 		const float Scale = FMath::InvSqrt(SquareSum);
// 		return Vector2D(X*Scale, Y*Scale);
// 	}
// 	return Vector2D(0.f, 0.f);
// }


// FORCEINLINE void Vector2D::Normalize(float Tolerance)
// {
// 	const float SquareSum = X*X + Y*Y;
// 	if(SquareSum > Tolerance)
// 	{
// 		const float Scale = FMath::InvSqrt(SquareSum);
// 		X *= Scale;
// 		Y *= Scale;
// 		return;
// 	}
// 	X = 0.0f;
// 	Y = 0.0f;
// }


FORCEINLINE void Vector2D::ToDirectionAndLength(Vector2D &OutDir, float &OutLength) const
{
	OutLength = Size();
	if (OutLength > SMALL_NUMBER)
	{
		float OneOverLength = 1.0f / OutLength;
		OutDir = Vector2D(X*OneOverLength, Y*OneOverLength);
	}
	else
	{
		OutDir = Vector2D::ZeroVector;
	}
}


FORCEINLINE bool Vector2D::IsNearlyZero(float Tolerance) const
{
	return	RMath::Abs(X)<=Tolerance
		&&	RMath::Abs(Y)<=Tolerance;
}


FORCEINLINE bool Vector2D::IsZero() const
{
	return X==0.f && Y==0.f;
}


FORCEINLINE float& Vector2D::Component(int Index)
{
	return (&X)[Index];
}


FORCEINLINE float Vector2D::Component(int Index) const
{
	return (&X)[Index];
}

// FORCEINLINE Vector2D Vector2D::RoundToVector() const
// {
// 	return Vector2D(FMath::RoundToFloat(X), FMath::RoundToFloat(Y));
// }

// FORCEINLINE Vector2D Vector2D::ClampAxes(float MinAxisVal, float MaxAxisVal) const
// {
// 	return Vector2D(FMath::Clamp(X, MinAxisVal, MaxAxisVal), FMath::Clamp(Y, MinAxisVal, MaxAxisVal));
// }


// FORCEINLINE Vector2D Vector2D::GetSignVector() const
// {
// 	return Vector2D
// 		(
// 		FMath::FloatSelect(X, 1.f, -1.f),
// 		FMath::FloatSelect(Y, 1.f, -1.f)
// 		);
// }

FORCEINLINE Vector2D Vector2D::GetAbs() const
{
	return Vector2D(RMath::Abs(X), RMath::Abs(Y));
}


FORCEINLINE std::string Vector2D::ToString() const
{
    std::ostringstream OS;
    OS << "X=" << X << " Y=" <<Y;
	return OS.str();
}


// FORCEINLINE bool Vector2D::InitFromString(const FString& InSourceString)
// {
// 	X = Y = 0;

// 	// The initialization is only successful if the X and Y values can all be parsed from the string
// 	const bool bSuccessful = FParse::Value(*InSourceString, TEXT("X=") , X) && FParse::Value(*InSourceString, TEXT("Y="), Y) ;

// 	return bSuccessful;
// }

/* FMath inline functions
 *****************************************************************************/

// FORCEINLINE float FMath::GetRangePct(Vector2D const& Range, float Value)
// {
// 	return GetRangePct(Range.X, Range.Y, Value);
// }

// FORCEINLINE float FMath::GetRangeValue(Vector2D const& Range, float Pct)
// {
// 	return Lerp<float>(Range.X, Range.Y, Pct);
// }
