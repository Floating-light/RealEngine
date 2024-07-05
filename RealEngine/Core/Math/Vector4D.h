#pragma once 

#include <string>
#include <sstream>

#include "Vector.h"
#include "Vector2D.h"

struct RVector4D;

using RColor = RVector4D;

struct RVector4D
{
public:

	/** The vector's X-component. */
	float X;

	/** The vector's Y-component. */
	float Y;

	/** The vector's Z-component. */
	float Z;

	/** The vector's W-component. */
	float W;

public:

	/**
	 * Constructor.
	 *
	 * @param InVector 3D Vector to set first three components.
	 * @param InW W Coordinate.
	 */
	RVector4D(const Vector3D& InVector, float InW = 1.0f);

	/**
	 * Creates and initializes a new vector from the specified components.
	 *
	 * @param InX X Coordinate.
	 * @param InY Y Coordinate.
	 * @param InZ Z Coordinate.
	 * @param InW W Coordinate.
	 */
	explicit RVector4D(float InX = 0.0f, float InY = 0.0f, float InZ = 0.0f, float InW = 1.0f);

	/**
	 * Creates and initializes a new vector from the specified 2D vectors.
	 *
	 * @param InXY A 2D vector holding the X- and Y-components.
	 * @param InZW A 2D vector holding the Z- and W-components.
	 */
	explicit RVector4D(Vector2D InXY, Vector2D InZW);

	explicit RVector4D();

public:

	/**
	 * Access a specific component of the vector.
	 *
	 * @param ComponentIndex The index of the component.
	 * @return Reference to the desired component.
	 */
	inline float& operator[](int ComponentIndex);

	/**
	 * Access a specific component of the vector.
	 *
	 * @param ComponentIndex The index of the component.
	 * @return Copy of the desired component.
	 */
	inline float operator[](int ComponentIndex) const;

	// Unary operators.

	inline operator DirectX::XMVECTOR() const { return DirectX::XMVectorSet(X, Y, Z, W); }

	/**
	 * Gets a negated copy of the vector.
	 *
	 * @return A negated copy of the vector.
	 */
	inline RVector4D operator-() const;

	/**
	 * Gets the result of adding a vector to this.
	 *
	 * @param V The vector to add.
	 * @return The result of vector addition.
	 */
	inline RVector4D operator+(const RVector4D& V) const;

	/**
	 * Adds another vector to this one.
	 *
	 * @param V The other vector to add.
	 * @return Copy of the vector after addition.
	 */
	inline RVector4D operator+=(const RVector4D& V);

	/**
	 * Gets the result of subtracting a vector from this.
	 *
	 * @param V The vector to subtract.
	 * @return The result of vector subtraction.
	 */
	inline RVector4D operator-(const RVector4D& V) const;

	/**
	 * Subtracts another vector to this one.
	 *
	 * @param V The other vector to subtract.
	 * @return Copy of the vector after subtraction.
	 */
	inline RVector4D operator-=(const RVector4D& V);

	/**
	 * Gets the result of scaling this vector.
	 *
	 * @param Scale The scaling factor.
	 * @return The result of vector scaling.
	 */
	inline RVector4D operator*(float Scale) const;

	/**
	 * Gets the result of dividing this vector.
	 *
	 * @param Scale What to divide by.
	 * @return The result of division.
	 */
	RVector4D operator/(float Scale) const;

	/**
	 * Gets the result of dividing this vector.
	 *
	 * @param V What to divide by.
	 * @return The result of division.
	 */
	RVector4D operator/(const RVector4D& V) const;

	/**
	 * Gets the result of multiplying a vector with this.
	 *
	 * @param V The vector to multiply.
	 * @return The result of vector multiplication.
	 */
	RVector4D operator*(const RVector4D& V) const;

	/**
	 * Gets the result of multiplying a vector with another Vector (component wise).
	 *
	 * @param V The vector to multiply.
	 * @return The result of vector multiplication.
	 */
	RVector4D operator*=(const RVector4D& V);

	/**
	 * Gets the result of dividing a vector with another Vector (component wise).
	 *
	 * @param V The vector to divide with.
	 * @return The result of vector multiplication.
	 */
	RVector4D operator/=(const RVector4D& V);

	/**
	 * Gets the result of scaling this vector.
	 *
	 * @param Scale The scaling factor.
	 * @return The result of vector scaling.
	 */
	RVector4D operator*=(float S);

	/**
	 * Calculates 3D Dot product of two 4D vectors.
	 *
	 * @param V1 The first vector.
	 * @param V2 The second vector.
	 * @return The 3D Dot product.
	 */
	friend inline float Dot3(const RVector4D& V1, const RVector4D& V2)
	{
		return V1.X*V2.X + V1.Y*V2.Y + V1.Z*V2.Z;
	}

	/**
	 * Calculates 4D Dot product.
	 *
	 * @param V1 The first vector.
	 * @param V2 The second vector.
	 * @return The 4D Dot Product.
	 */
	friend inline float Dot4(const RVector4D& V1, const RVector4D& V2)
	{
		return V1.X*V2.X + V1.Y*V2.Y + V1.Z*V2.Z + V1.W*V2.W;
	}

	/**
	 * Scales a vector.
	 *
	 * @param Scale The scaling factor.
	 * @param V The vector to scale.
	 * @return The result of scaling.
	 */
	friend inline RVector4D operator*(float Scale, const RVector4D& V)
	{
		return V.operator*(Scale);
	}

	/**
	 * Checks for equality against another vector.
	 *
	 * @param V The other vector.
	 * @return true if the two vectors are the same, otherwise false.
	 */
	bool operator==(const RVector4D& V) const;

	/**
	 * Checks for inequality against another vector.
	 *
	 * @param V The other vector.
	 * @return true if the two vectors are different, otherwise false.
	 */
	bool operator!=(const RVector4D& V) const;

	/**
	 * Calculate Cross product between this and another vector.
	 *
	 * @param V The other vector.
	 * @return The Cross product.
	 */
	RVector4D operator^(const RVector4D& V) const;

public:

	// Simple functions.

	/**
	 * Gets a specific component of the vector.
	 *
	 * @param Index The index of the component.
	 * @return Reference to the component.
	 */
	float& Component(int Index);

	/**
	* Gets a specific component of the vector.
	*
	* @param Index The index of the component.
	* @return Reference to the component.
	*/
	const float& Component(int Index) const;

	/**
	 * Error tolerant comparison.
	 *
	 * @param V Vector to compare against.
	 * @param Tolerance Error Tolerance.
	 * @return true if the two vectors are equal within specified tolerance, otherwise false.
	 */
	bool Equals(const RVector4D& V, float Tolerance=KINDA_SMALL_NUMBER) const;

	/**
	 * Check if the vector is of unit length, with specified tolerance.
	 *
	 * @param LengthSquaredTolerance Tolerance against squared length.
	 * @return true if the vector is a unit vector within the specified tolerance.
	 */
	bool IsUnit3(float LengthSquaredTolerance = KINDA_SMALL_NUMBER) const;

	/**
	 * Get a textual representation of the vector.
	 *
	 * @return Text describing the vector.
	 */
	std::string ToString() const;

	/**
	 * Initialize this Vector based on an std::string. The String is expected to contain X=, Y=, Z=, W=.
	 * The RVector4D will be bogus when InitFromString returns false.
	 *
	 * @param InSourceString	std::string containing the vector values.
	 * @return true if the X,Y,Z values were read successfully; false otherwise.
	 */
	bool InitFromString(const std::string& InSourceString);

	/**
	 * Returns a normalized copy of the vector if safe to normalize.
	 *
	 * @param Tolerance Minimum squared length of vector for normalization.
	 * @return A normalized copy of the vector or a zero vector.
	 */
	inline RVector4D GetSafeNormal(float Tolerance = SMALL_NUMBER) const;

	/**
	 * Calculates normalized version of vector without checking if it is non-zero.
	 *
	 * @return Normalized version of vector.
	 */
	inline RVector4D GetUnsafeNormal3() const;

	/**
	 * Set all of the vectors coordinates.
	 *
	 * @param InX New X Coordinate.
	 * @param InY New Y Coordinate.
	 * @param InZ New Z Coordinate.
	 * @param InW New W Coordinate.
	 */
	inline void Set(float InX, float InY, float InZ, float InW);

	/**
	 * Get the length of this vector not taking W component into account.
	 *
	 * @return The length of this vector.
	 */
	float Size3() const;

	/**
	 * Get the squared length of this vector not taking W component into account.
	 *
	 * @return The squared length of this vector.
	 */
	float SizeSquared3() const;

	/**
	 * Get the length (magnitude) of this vector, taking the W component into account
	 *
	 * @return The length of this vector
	 */
	float Size() const;

	/**
	 * Get the squared length of this vector, taking the W component into account
	 *
	 * @return The squared length of this vector
	 */
	float SizeSquared() const;

	/** Utility to check if there are any non-finite values (NaN or Inf) in this vector. */
	bool ContainsNaN() const;

	/** Utility to check if all of the components of this vector are nearly zero given the tolerance. */
	bool IsNearlyZero3(float Tolerance = KINDA_SMALL_NUMBER) const;

	/** Reflect vector. */
	RVector4D Reflect3(const RVector4D& Normal) const;

	/**
	 * Find good arbitrary axis vectors to represent U and V axes of a plane,
	 * given just the normal.
	 */
	void FindBestAxisVectors3(RVector4D& Axis1, RVector4D& Axis2) const;

#if ENABLE_NAN_DIAGNOSTIC
	inline void DiagnosticCheckNaN()
	{
		if (ContainsNaN())
		{
			logOrEnsureNanError(TEXT("Vector3D contains NaN: %s"), *ToString());
			*this = RVector4D(Vector3D::ZeroVector);

		}
	}
#else
	inline void DiagnosticCheckNaN() { }
#endif
	static const RColor White;
	static const RColor Gray;
	static const RColor Black;
	static const RColor Transparent;
	static const RColor Red;
	static const RColor Green;
	static const RColor Blue;
	static const RColor Yellow ;
};

/* RVector4D inline functions
 *****************************************************************************/

inline RVector4D::RVector4D(const Vector3D& InVector,float InW)
	: X(InVector.X)
	, Y(InVector.Y)
	, Z(InVector.Z)
	, W(InW)
{
	DiagnosticCheckNaN();
}

inline RVector4D::RVector4D(float InX,float InY,float InZ,float InW)
	: X(InX)
	, Y(InY)
	, Z(InZ)
	, W(InW)
{
	DiagnosticCheckNaN();
}


inline RVector4D::RVector4D()
	: X(0.f)
	, Y(0.f)
	, Z(0.f)
	, W(0.f)
{
	DiagnosticCheckNaN();
}

inline RVector4D::RVector4D(Vector2D InXY, Vector2D InZW)
	: X(InXY.X)
	, Y(InXY.Y)
	, Z(InZW.X)
	, W(InZW.Y)
{
	DiagnosticCheckNaN();
}

inline float& RVector4D::operator[](int ComponentIndex)
{
	return (&X)[ ComponentIndex ];
}


inline float RVector4D::operator[](int ComponentIndex) const
{
	return (&X)[ ComponentIndex ];
}


inline void RVector4D::Set(float InX, float InY, float InZ, float InW)
{
	X = InX;
	Y = InY;
	Z = InZ;
	W = InW;

	DiagnosticCheckNaN();
}


inline RVector4D RVector4D::operator-() const
{
	return RVector4D(-X, -Y, -Z, -W);
}


inline RVector4D RVector4D::operator+(const RVector4D& V) const
{
	return RVector4D(X + V.X, Y + V.Y, Z + V.Z, W + V.W);
}


inline RVector4D RVector4D::operator+=(const RVector4D& V)
{
	X += V.X; Y += V.Y; Z += V.Z; W += V.W;
	DiagnosticCheckNaN();
	return *this;
}


inline RVector4D RVector4D::operator-(const RVector4D& V) const
{
	return RVector4D(X - V.X, Y - V.Y, Z - V.Z, W - V.W);
}


inline RVector4D RVector4D::operator-=(const RVector4D& V)
{
	X -= V.X; Y -= V.Y; Z -= V.Z; W -= V.W;
	DiagnosticCheckNaN();
	return *this;
}


inline RVector4D RVector4D::operator*(float Scale) const
{
	return RVector4D(X * Scale, Y * Scale, Z * Scale, W * Scale);
}


inline RVector4D RVector4D::operator/(float Scale) const
{
	const float RScale = 1.f/Scale;
	return RVector4D(X * RScale, Y * RScale, Z * RScale, W * RScale);
}


inline RVector4D RVector4D::operator*(const RVector4D& V) const
{
	return RVector4D(X * V.X, Y * V.Y, Z * V.Z, W * V.W);
}


inline RVector4D RVector4D::operator^(const RVector4D& V) const
{
	return RVector4D(
		Y * V.Z - Z * V.Y,
		Z * V.X - X * V.Z,
		X * V.Y - Y * V.X,
		0.0f
	);
}


inline float& RVector4D::Component(int Index)
{
	return (&X)[Index];
}

inline const float& RVector4D::Component(int Index) const
{
	return (&X)[Index];
}

inline bool RVector4D::operator==(const RVector4D& V) const
{
	return ((X == V.X) && (Y == V.Y) && (Z == V.Z) && (W == V.W));
}


inline bool RVector4D::operator!=(const RVector4D& V) const
{
	return ((X != V.X) || (Y != V.Y) || (Z != V.Z) || (W != V.W));
}


inline bool RVector4D::Equals(const RVector4D& V, float Tolerance) const
{
	return RMath::Abs(X-V.X) <= Tolerance && RMath::Abs(Y-V.Y) <= Tolerance && RMath::Abs(Z-V.Z) <= Tolerance && RMath::Abs(W-V.W) <= Tolerance;
}


inline std::string RVector4D::ToString() const
{
    std::ostringstream os;
    os << "X=" << X << " Y=" << Y << " Z=" << Z << " W=" << W;
	return os.str();
}


inline bool RVector4D::InitFromString(const std::string& InSourceString)
{
	X = Y = Z = 0;
	W = 1.0f;
	return true;
}


inline RVector4D RVector4D::GetSafeNormal(float Tolerance) const
{
	const float SquareSum = X*X + Y*Y + Z*Z;
	if(SquareSum > Tolerance)
	{
		const float Scale = RMath::InvSqrt(SquareSum);
		return RVector4D(X*Scale, Y*Scale, Z*Scale, 0.0f);
	}
	return RVector4D(0.f);
}


inline RVector4D RVector4D::GetUnsafeNormal3() const
{
	const float Scale = RMath::InvSqrt(X*X+Y*Y+Z*Z);
	return RVector4D(X*Scale, Y*Scale, Z*Scale, 0.0f);
}


inline float RVector4D::Size3() const
{
	return RMath::Sqrt(X*X + Y*Y + Z*Z);
}


inline float RVector4D::SizeSquared3() const
{
	return X*X + Y*Y + Z*Z;
}

inline float RVector4D::Size() const
{
	return RMath::Sqrt(X*X + Y*Y + Z*Z + W*W);
}

inline float RVector4D::SizeSquared() const
{
	return X*X + Y*Y + Z*Z + W*W;
}


inline bool RVector4D::IsUnit3(float LengthSquaredTolerance) const
{
	return RMath::Abs(1.0f - SizeSquared3()) < LengthSquaredTolerance;
}


inline bool RVector4D::ContainsNaN() const
{
	return (!RMath::IsFinite(X) || 
			!RMath::IsFinite(Y) ||
			!RMath::IsFinite(Z) ||
			!RMath::IsFinite(W));
}


inline bool RVector4D::IsNearlyZero3(float Tolerance) const
{
	return
			RMath::Abs(X)<=Tolerance
		&&	RMath::Abs(Y)<=Tolerance
		&&	RMath::Abs(Z)<=Tolerance;
}


inline RVector4D RVector4D::Reflect3(const RVector4D& Normal) const
{
	return 2.0f * Dot3(*this, Normal) * Normal - *this;
}


inline void RVector4D::FindBestAxisVectors3(RVector4D& Axis1, RVector4D& Axis2) const
{
	const float NX = RMath::Abs(X);
	const float NY = RMath::Abs(Y);
	const float NZ = RMath::Abs(Z);

	// Find best basis vectors.
	if(NZ>NX && NZ>NY)	Axis1 = RVector4D(1,0,0);
	else					Axis1 = RVector4D(0,0,1);

	Axis1 = (Axis1 - *this * Dot3(Axis1, *this)).GetSafeNormal();
	Axis2 = Axis1 ^ *this;
}


inline RVector4D RVector4D::operator*=(const RVector4D& V)
{
	X *= V.X; Y *= V.Y; Z *= V.Z; W *= V.W;
	DiagnosticCheckNaN();
	return *this;
}


inline RVector4D RVector4D::operator/=(const RVector4D& V)
{
	X /= V.X; Y /= V.Y; Z /= V.Z; W /= V.W;
	DiagnosticCheckNaN();
	return *this;
}


inline RVector4D RVector4D::operator*=(float S)
{
	X *= S; Y *= S; Z *= S; W *= S;
	DiagnosticCheckNaN();
	return *this;
}


inline RVector4D RVector4D::operator/(const RVector4D& V) const
{
	return RVector4D(X / V.X, Y / V.Y, Z / V.Z, W / V.W);
}

// template <> struct TIsPODType<RVector4D> { enum { Value = true }; };


// /* Vector3D inline functions
//  *****************************************************************************/

// inline Vector3D::Vector3D( const RVector4D& V )
// 	: X(V.X), Y(V.Y), Z(V.Z)
// {
// 	DiagnosticCheckNaN();
// }

// /* Vector3D2D inline functions
//  *****************************************************************************/

// inline Vector3D2D::Vector3D2D(const RVector4D& V)
// 	: X(V.X), Y(V.Y)
// {
// }