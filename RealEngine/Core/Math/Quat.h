#pragma once 

#include <string>

#include "MathUtilityDefine.h"
#include "Vector.h"
#include "Rotator.h"
#include "Matrix4.h"

class Error;

/**
 * Floating point quaternion that can represent a rotation about an axis in 3-D space.
 * The X, Y, Z, W components also double as the Axis/Angle format.
 *
 * Order matters when composing quaternions: C = A * B will yield a quaternion C that logically
 * first applies B then A to any subsequent transformation (right first, then left).
 * Note that this is the opposite order of FTransform multiplication.
 *
 * Example: LocalToWorld = (LocalToWorld * DeltaRotation) will change rotation in local space by DeltaRotation.
 * Example: LocalToWorld = (DeltaRotation * LocalToWorld) will change rotation in world space by DeltaRotation.
 */
__declspec(align(16)) struct RQuat 
{
public:

	/** The quaternion's X-component. */
	float X;

	/** The quaternion's Y-component. */
	float Y;

	/** The quaternion's Z-component. */
	float Z;

	/** The quaternion's W-component. */
	float W;

public:

	/** Identity quaternion. */
	static const RQuat Identity;

public:

	/** Default constructor (no initialization). */
	inline RQuat() { }

	/**
	 * Creates and initializes a new quaternion, with the W component either 0 or 1.
	 *
	 * @param EForceInit Force init enum: if equal to ForceInitToZero then W is 0, otherwise W = 1 (creating an identity transform)
	 */
	explicit inline RQuat(EForceInit);

	/**
	 * Constructor.
	 *
	 * @param InX X component of the quaternion
	 * @param InY Y component of the quaternion
	 * @param InZ Z component of the quaternion
	 * @param InW W component of the quaternion
	 */
	inline RQuat(float InX, float InY, float InZ, float InW);

	/**
	 * Creates and initializes a new quaternion from the XYZW values in the given VectorRegister.
	 *
	 * @param V XYZW components of the quaternion packed into a single VectorRegister
	 */
	explicit inline RQuat(const VectorRegister& V);

	/**
	 * Creates and initializes a new quaternion from the given matrix.
	 *
	 * @param M The rotation matrix to initialize from.
	 */
	explicit RQuat(const Matrix4& M);	

	/**
	 * Creates and initializes a new quaternion from the given rotator.
	 *
	 * @param R The rotator to initialize from.
	 */
	explicit RQuat(const Rotator& R);

	/**
	 * Creates and initializes a new quaternion from the a rotation around the given axis.
	 *
	 * @param Axis assumed to be a normalized vector
	 * @param Angle angle to rotate above the given axis (in radians)
	 */
	RQuat(Vector Axis, float AngleRad);

public:

#ifdef IMPLEMENT_ASSIGNMENT_OPERATOR_MANUALLY
	/**
	 * Copy another RQuat into this one
	 *
	 * @return reference to this RQuat
	 */
	inline RQuat& operator=(const RQuat& Other);
#endif

	/**
	 * Gets the result of adding a Quaternion to this.
	 * This is a component-wise addition; composing quaternions should be done via multiplication.
	 *
	 * @param Q The Quaternion to add.
	 * @return The result of addition.
	 */
	inline RQuat operator+(const RQuat& Q) const;

	/**
	 * Adds to this quaternion.
	 * This is a component-wise addition; composing quaternions should be done via multiplication.
	 *
	 * @param Other The quaternion to add to this.
	 * @return Result after addition.
	 */
	inline RQuat operator+=(const RQuat& Q);

	/**
	 * Gets the result of subtracting a Quaternion to this.
	 * This is a component-wise subtraction; composing quaternions should be done via multiplication.
	 *
	 * @param Q The Quaternion to subtract.
	 * @return The result of subtraction.
	 */
	inline RQuat operator-(const RQuat& Q) const;

	/**
	 * Checks whether another Quaternion is equal to this, within specified tolerance.
	 *
	 * @param Q The other Quaternion.
	 * @param Tolerance Error tolerance for comparison with other Quaternion.
	 * @return true if two Quaternions are equal, within specified tolerance, otherwise false.
	 */
	inline bool Equals(const RQuat& Q, float Tolerance=KINDA_SMALL_NUMBER) const;

	/**
	 * Checks whether this Quaternion is an Identity Quaternion.
	 * Assumes Quaternion tested is normalized.
	 *
	 * @param Tolerance Error tolerance for comparison with Identity Quaternion.
	 * @return true if Quaternion is a normalized Identity Quaternion.
	 */
	inline bool IsIdentity(float Tolerance=SMALL_NUMBER) const;

	/**
	 * Subtracts another quaternion from this.
	 * This is a component-wise subtraction; composing quaternions should be done via multiplication.
	 *
	 * @param Q The other quaternion.
	 * @return reference to this after subtraction.
	 */
	inline RQuat operator-=(const RQuat& Q);

	/**
	 * Gets the result of multiplying this by another quaternion (this * Q).
	 *
	 * Order matters when composing quaternions: C = A * B will yield a quaternion C that logically
	 * first applies B then A to any subsequent transformation (right first, then left).
	 *
	 * @param Q The Quaternion to multiply this by.
	 * @return The result of multiplication (this * Q).
	 */
	inline RQuat operator*(const RQuat& Q) const;

	/**
	 * Multiply this by a quaternion (this = this * Q).
	 *
	 * Order matters when composing quaternions: C = A * B will yield a quaternion C that logically
	 * first applies B then A to any subsequent transformation (right first, then left).
	 *
	 * @param Q the quaternion to multiply this by.
	 * @return The result of multiplication (this * Q).
	 */
	inline RQuat operator*=(const RQuat& Q);

	/**
	 * Rotate a vector by this quaternion.
	 *
	 * @param V the vector to be rotated
	 * @return vector after rotation
	 * @see RotateVector
	 */
	Vector operator*(const Vector& V) const;

	/** 
	 * Multiply this by a matrix.
	 * This matrix conversion came from
	 * http://www.m-hikari.com/ija/ija-password-2008/ija-password17-20-2008/aristidouIJA17-20-2008.pdf
	 * used for non-uniform scaling transform.
	 *
	 * @param M Matrix to multiply by.
	 * @return Matrix result after multiplication.
	 */	
	Matrix4 operator*(const Matrix4& M) const;
	
	/**
	 * Multiply this quaternion by a scaling factor.
	 *
	 * @param Scale The scaling factor.
	 * @return a reference to this after scaling.
	 */
	inline RQuat operator*=(const float Scale);

	/**
	 * Get the result of scaling this quaternion.
	 *
	 * @param Scale The scaling factor.
	 * @return The result of scaling.
	 */
	inline RQuat operator*(const float Scale) const;
	
	/**
	 * Divide this quaternion by scale.
	 *
	 * @param Scale What to divide by.
	 * @return a reference to this after scaling.
	 */
	inline RQuat operator/=(const float Scale);

	/**
	 * Divide this quaternion by scale.
	 *
	 * @param Scale What to divide by.
	 * @return new Quaternion of this after division by scale.
	 */
	inline RQuat operator/(const float Scale) const;

	/**
	 * Identical implementation for RQuat properties. 
	 * Avoids intrinsics to remain consistent with previous per-property comparison.
	 */
	bool Identical(const RQuat* Q, const int PortFlags) const;

 	/**
	 * Checks whether two quaternions are identical.
	 * This is an exact comparison per-component;see Equals() for a comparison
	 * that allows for a small error tolerance and flipped axes of rotation.
	 *
	 * @param Q The other quaternion.
	 * @return true if two quaternion are identical, otherwise false.
	 * @see Equals
	 */
	bool operator==(const RQuat& Q) const;

 	/**
	 * Checks whether two quaternions are not identical.
	 *
	 * @param Q The other quaternion.
	 * @return true if two quaternion are not identical, otherwise false.
	 */
	bool operator!=(const RQuat& Q) const;

	/**
	 * Calculates dot product of two quaternions.
	 *
	 * @param Q The other quaternions.
	 * @return The dot product.
	 */
	float operator|(const RQuat& Q) const;

public:

	/**
	 * Convert a vector of floating-point Euler angles (in degrees) into a Quaternion.
	 * 
	 * @param Euler the Euler angles
	 * @return constructed RQuat
	 */
	static  RQuat MakeFromEuler(const Vector& Euler);

	/** Convert a Quaternion into floating-point Euler angles (in degrees). */
	 Vector Euler() const;

	/**
	 * Normalize this quaternion if it is large enough.
	 * If it is too small, returns an identity quaternion.
	 *
	 * @param Tolerance Minimum squared length of quaternion for normalization.
	 */
	inline void Normalize(float Tolerance = SMALL_NUMBER);

	/**
	 * Get a normalized copy of this quaternion.
	 * If it is too small, returns an identity quaternion.
	 *
	 * @param Tolerance Minimum squared length of quaternion for normalization.
	 */
	inline RQuat GetNormalized(float Tolerance = SMALL_NUMBER) const;

	// Return true if this quaternion is normalized
	bool IsNormalized() const;

	/**
	 * Get the length of this quaternion.
	 *
	 * @return The length of this quaternion.
	 */
	inline float Size() const;

	/**
	 * Get the length squared of this quaternion.
	 *
	 * @return The length of this quaternion.
	 */
	inline float SizeSquared() const;


	/** Get the angle of this quaternion */
	inline float GetAngle() const;

	/** 
	 * get the axis and angle of rotation of this quaternion
	 *
	 * @param Axis{out] vector of axis of the quaternion
	 * @param Angle{out] angle of the quaternion
	 * @warning : assumes normalized quaternions.
	 */
	void ToAxisAndAngle(Vector& Axis, float& Angle) const;

	/** 
	 * Get the swing and twist decomposition for a specified axis
	 *
	 * @param InTwistAxis Axis to use for decomposition
	 * @param OutSwing swing component quaternion
	 * @param OutTwist Twist component quaternion
	 * @warning assumes normalized quaternion and twist axis
	 */
	 void ToSwingTwist(const Vector& InTwistAxis, RQuat& OutSwing, RQuat& OutTwist) const;

	/**
	 * Get the twist angle (in radians) for a specified axis
	 *
	 * @param TwistAxis Axis to use for decomposition
	 * @return Twist angle (in radians)
	 * @warning assumes normalized quaternion and twist axis
	 */
	 float GetTwistAngle(const Vector& TwistAxis) const;

	/**
	 * Rotate a vector by this quaternion.
	 *
	 * @param V the vector to be rotated
	 * @return vector after rotation
	 */
	Vector RotateVector(Vector V) const;
	
	/**
	 * Rotate a vector by the inverse of this quaternion.
	 *
	 * @param V the vector to be rotated
	 * @return vector after rotation by the inverse of this quaternion.
	 */
	Vector UnrotateVector(Vector V) const;

	/**
	 * @return quaternion with W=0 and V=theta*v.
	 */
	 RQuat Log() const;

	/**
	 * @note Exp should really only be used after Log.
	 * Assumes a quaternion with W=0 and V=theta*v (where |v| = 1).
	 * Exp(q) = (sin(theta)*v, cos(theta))
	 */
	 RQuat Exp() const;

	/**
	 * @return inverse of this quaternion
	 */
	inline RQuat Inverse() const;

	/**
	 * Enforce that the delta between this Quaternion and another one represents
	 * the shortest possible rotation angle
	 */
	void EnforceShortestArcWith(const RQuat& OtherQuat);
	
	/** Get the forward direction (X axis) after it has been rotated by this Quaternion. */
	inline Vector GetAxisX() const;

	/** Get the right direction (Y axis) after it has been rotated by this Quaternion. */
	inline Vector GetAxisY() const;

	/** Get the up direction (Z axis) after it has been rotated by this Quaternion. */
	inline Vector GetAxisZ() const;

	/** Get the forward direction (X axis) after it has been rotated by this Quaternion. */
	inline Vector GetForwardVector() const;

	/** Get the right direction (Y axis) after it has been rotated by this Quaternion. */
	inline Vector GetRightVector() const;

	/** Get the up direction (Z axis) after it has been rotated by this Quaternion. */
	inline Vector GetUpVector() const;

	/** Convert a rotation into a unit vector facing in its direction. Equivalent to GetForwardVector(). */
	inline Vector ToVector() const;

	/** Get the Rotator representation of this Quaternion. */
	 Rotator Rotator() const;

	/**
	 * Get the axis of rotation of the Quaternion.
	 * This is the axis around which rotation occurs to transform the canonical coordinate system to the target orientation.
	 * For the identity Quaternion which has no such rotation, Vector(1,0,0) is returned.
	 */
	inline Vector GetRotationAxis() const;

	/** Find the angular distance between two rotation quaternions (in radians) */
	inline float AngularDistance(const RQuat& Q) const;

	/**
	 * Utility to check if there are any non-finite values (NaN or Inf) in this Quat.
	 *
	 * @return true if there are any non-finite values in this Quaternion, otherwise false.
	 */
	bool ContainsNaN() const;

	/**
	 * Get a textual representation of the vector.
	 *
	 * @return Text describing the vector.
	 */
	std::string ToString() const;

	/**
	 * Initialize this RQuat from a std::string. 
	 * The string is expected to contain X=, Y=, Z=, W=, otherwise 
	 * this RQuat will have indeterminate (invalid) values.
	 *
	 * @param InSourceString std::string containing the quaternion values.
	 * @return true if the RQuat was initialized; false otherwise.
	 */
	bool InitFromString(const std::string& InSourceString);

public:

#if ENABLE_NAN_DIAGNOSTIC
	inline void DiagnosticCheckNaN() const
	{
		if (ContainsNaN())
		{
			logOrEnsureNanError(TEXT("RQuat contains NaN: %s"), *ToString());
			*const_cast<RQuat*>(this) = RQuat::Identity;
		}
	}

	inline void DiagnosticCheckNaN(const TCHAR* Message) const
	{
		if (ContainsNaN())
		{
			logOrEnsureNanError(TEXT("%s: RQuat contains NaN: %s"), Message, *ToString());
			*const_cast<RQuat*>(this) = RQuat::Identity;
		}
	}
#else
	inline void DiagnosticCheckNaN() const {}
	inline void DiagnosticCheckNaN(const wchar_t* Message) const {}
#endif

public:

	/**
	 * Generates the 'smallest' (geodesic) rotation between two vectors of arbitrary length.
	 */
	static inline RQuat FindBetween(const Vector& Vector1, const Vector& Vector2)
	{
		return FindBetweenVectors(Vector1, Vector2);
	}

	/**
	 * Generates the 'smallest' (geodesic) rotation between two normals (assumed to be unit length).
	 */
	static  RQuat FindBetweenNormals(const Vector& Normal1, const Vector& Normal2);

	/**
	 * Generates the 'smallest' (geodesic) rotation between two vectors of arbitrary length.
	 */
	static  RQuat FindBetweenVectors(const Vector& Vector1, const Vector& Vector2);

	/**
	 * Error measure (angle) between two quaternions, ranged [0..1].
	 * Returns the hypersphere-angle between two quaternions; alignment shouldn't matter, though 
	 * @note normalized input is expected.
	 */
	static inline float Error(const RQuat& Q1, const RQuat& Q2);

	/**
	 * RQuat::Error with auto-normalization.
	 */
	static inline float ErrorAutoNormalize(const RQuat& A, const RQuat& B);

	/** 
	 * Fast Linear Quaternion Interpolation.
	 * Result is NOT normalized.
	 */
	static inline RQuat FastLerp(const RQuat& A, const RQuat& B, const float Alpha);

	/** 
	 * Bi-Linear Quaternion interpolation.
	 * Result is NOT normalized.
	 */
	static inline RQuat FastBilerp(const RQuat& P00, const RQuat& P10, const RQuat& P01, const RQuat& P11, float FracX, float FracY);


	/** Spherical interpolation. Will correct alignment. Result is NOT normalized. */
	static  RQuat Slerp_NotNormalized(const RQuat &Quat1, const RQuat &Quat2, float Slerp);

	/** Spherical interpolation. Will correct alignment. Result is normalized. */
	static inline RQuat Slerp(const RQuat &Quat1, const RQuat &Quat2, float Slerp)
	{
		return Slerp_NotNormalized(Quat1, Quat2, Slerp).GetNormalized();
	}

	/**
	 * Simpler Slerp that doesn't do any checks for 'shortest distance' etc.
	 * We need this for the cubic interpolation stuff so that the multiple Slerps dont go in different directions.
	 * Result is NOT normalized.
	 */
	static  RQuat SlerpFullPath_NotNormalized(const RQuat &quat1, const RQuat &quat2, float Alpha);

	/**
	 * Simpler Slerp that doesn't do any checks for 'shortest distance' etc.
	 * We need this for the cubic interpolation stuff so that the multiple Slerps dont go in different directions.
	 * Result is normalized.
	 */
	static inline RQuat SlerpFullPath(const RQuat &quat1, const RQuat &quat2, float Alpha)
	{
		return SlerpFullPath_NotNormalized(quat1, quat2, Alpha).GetNormalized();
	}
	
	/**
	 * Given start and end quaternions of quat1 and quat2, and tangents at those points tang1 and tang2, calculate the point at Alpha (between 0 and 1) between them. Result is normalized.
	 * This will correct alignment by ensuring that the shortest path is taken.
	 */
	static  RQuat Squad(const RQuat& quat1, const RQuat& tang1, const RQuat& quat2, const RQuat& tang2, float Alpha);

	/**
	 * Simpler Squad that doesn't do any checks for 'shortest distance' etc.
	 * Given start and end quaternions of quat1 and quat2, and tangents at those points tang1 and tang2, calculate the point at Alpha (between 0 and 1) between them. Result is normalized.
	 */
	static  RQuat SquadFullPath(const RQuat& quat1, const RQuat& tang1, const RQuat& quat2, const RQuat& tang2, float Alpha);

	/**
	 * Calculate tangents between given points
	 *
	 * @param PrevP quaternion at P-1
	 * @param P quaternion to return the tangent
	 * @param NextP quaternion P+1
	 * @param Tension @todo document
	 * @param OutTan Out control point
	 */
	static  void CalcTangents(const RQuat& PrevP, const RQuat& P, const RQuat& NextP, float Tension, RQuat& OutTan);

} ;


/* RQuat inline functions
 *****************************************************************************/

inline RQuat::RQuat(const Matrix4& M)
{
// 	// If Matrix is NULL, return Identity quaternion. If any of them is 0, you won't be able to construct rotation
// 	// if you have two plane at least, we can reconstruct the frame using cross product, but that's a bit expensive op to do here
// 	// for now, if you convert to matrix from 0 scale and convert back, you'll lose rotation. Don't do that. 
// 	if (M.GetScaledAxis(EAxis::X).IsNearlyZero() || M.GetScaledAxis(EAxis::Y).IsNearlyZero() || M.GetScaledAxis(EAxis::Z).IsNearlyZero())
// 	{
// 		*this = RQuat::Identity;
// 		return;
// 	}

// #if !(UE_BUILD_SHIPPING || UE_BUILD_TEST)
// 	// Make sure the Rotation part of the Matrix is unit length.
// 	// Changed to this (same as RemoveScaling) from RotDeterminant as using two different ways of checking unit length matrix caused inconsistency. 
// 	if (!ensure((RMath::Abs(1.f - M.GetScaledAxis(EAxis::X).SizeSquared()) <= KINDA_SMALL_NUMBER) && (RMath::Abs(1.f - M.GetScaledAxis(EAxis::Y).SizeSquared()) <= KINDA_SMALL_NUMBER) && (RMath::Abs(1.f - M.GetScaledAxis(EAxis::Z).SizeSquared()) <= KINDA_SMALL_NUMBER)))
// 	{
// 		*this = RQuat::Identity;
// 		return;
// 	}
// #endif

// 	//const MeReal *const t = (MeReal *) tm;
// 	float	s;

// 	// Check diagonal (trace)
// 	const float tr = M.M[0][0] + M.M[1][1] + M.M[2][2];

// 	if (tr > 0.0f) 
// 	{
// 		float InvS = RMath::InvSqrt(tr + 1.f);
// 		this->W = 0.5f * (1.f / InvS);
// 		s = 0.5f * InvS;

// 		this->X = (M.M[1][2] - M.M[2][1]) * s;
// 		this->Y = (M.M[2][0] - M.M[0][2]) * s;
// 		this->Z = (M.M[0][1] - M.M[1][0]) * s;
// 	} 
// 	else 
// 	{
// 		// diagonal is negative
// 		int32 i = 0;

// 		if (M.M[1][1] > M.M[0][0])
// 			i = 1;

// 		if (M.M[2][2] > M.M[i][i])
// 			i = 2;

// 		static const int32 nxt[3] = { 1, 2, 0 };
// 		const int32 j = nxt[i];
// 		const int32 k = nxt[j];
 
// 		s = M.M[i][i] - M.M[j][j] - M.M[k][k] + 1.0f;

// 		float InvS = RMath::InvSqrt(s);

// 		float qt[4];
// 		qt[i] = 0.5f * (1.f / InvS);

// 		s = 0.5f * InvS;

// 		qt[3] = (M.M[j][k] - M.M[k][j]) * s;
// 		qt[j] = (M.M[i][j] + M.M[j][i]) * s;
// 		qt[k] = (M.M[i][k] + M.M[k][i]) * s;

// 		this->X = qt[0];
// 		this->Y = qt[1];
// 		this->Z = qt[2];
// 		this->W = qt[3];

// 		DiagnosticCheckNaN();
// 	}
}


inline RQuat::RQuat(const Rotator& R)
{
	*this = R.Quaternion();
	DiagnosticCheckNaN();
}


inline Vector RQuat::operator*(const Vector& V) const
{
	return RotateVector(V);
}


inline Matrix4 RQuat::operator*(const Matrix4& M) const
{
	Matrix4 Result;
	RQuat VT, VR;
	RQuat Inv = Inverse();
	for (int I=0; I<4; ++I)
	{
		RQuat VQ(M.Mat[I][0], M.Mat[I][1], M.Mat[I][2], M.Mat[I][3]);
		VectorQuaternionMultiply(&VT, this, &VQ);
		VectorQuaternionMultiply(&VR, &VT, &Inv);
		Result.Mat[I][0] = VR.X;
		Result.Mat[I][1] = VR.Y;
		Result.Mat[I][2] = VR.Z;
		Result.Mat[I][3] = VR.W;
	}

	return Result;
}


/* RQuat inline functions
 *****************************************************************************/

inline RQuat::RQuat(EForceInit ZeroOrNot)
	:	X(0), Y(0), Z(0), W(ZeroOrNot == ForceInitToZero ? 0.0f : 1.0f)
{ }


inline RQuat::RQuat(float InX, float InY, float InZ, float InW)
	: X(InX)
	, Y(InY)
	, Z(InZ)
	, W(InW)
{
	DiagnosticCheckNaN();
}


inline RQuat::RQuat(const VectorRegister& V)
{
	VectorStoreAligned(V, this);
	DiagnosticCheckNaN();
}


inline std::string RQuat::ToString() const
{
	// return std::string::Printf(TEXT("X=%.9f Y=%.9f Z=%.9f W=%.9f"), X, Y, Z, W);
    return std::string("TODO");
}

inline bool RQuat::InitFromString(const std::string& InSourceString)
{
	X = Y = Z = 0.f;
	W = 1.f;

	// const bool bSuccess
	// 	=  FParse::Value(*InSourceString, TEXT("X="), X)
	// 	&& FParse::Value(*InSourceString, TEXT("Y="), Y)
	// 	&& FParse::Value(*InSourceString, TEXT("Z="), Z)
	// 	&& FParse::Value(*InSourceString, TEXT("W="), W);
	DiagnosticCheckNaN();
	return false;
}

#ifdef IMPLEMENT_ASSIGNMENT_OPERATOR_MANUALLY
inline RQuat& RQuat::operator=(const RQuat& Other)
{
	this->X = Other.X;
	this->Y = Other.Y;
	this->Z = Other.Z;
	this->W = Other.W;

	return *this;
}
#endif


inline RQuat::RQuat(Vector Axis, float AngleRad)
{
	const float half_a = 0.5f * AngleRad;
	float s, c;
	RMath::SinCos(&s, &c, half_a);

	X = s * Axis.X;
	Y = s * Axis.Y;
	Z = s * Axis.Z;
	W = c;

	DiagnosticCheckNaN();
}


inline RQuat RQuat::operator+(const RQuat& Q) const
{
#if PLATFORM_ENABLE_VECTORINTRINSICS
	VectorRegister A = VectorLoadAligned(this);
	VectorRegister B = VectorLoadAligned(&Q);
	return RQuat(VectorAdd(A, B));
#else
	return RQuat(X + Q.X, Y + Q.Y, Z + Q.Z, W + Q.W);
#endif // PLATFORM_ENABLE_VECTORINTRINSICS
}


inline RQuat RQuat::operator+=(const RQuat& Q)
{
#if PLATFORM_ENABLE_VECTORINTRINSICS
	VectorRegister A = VectorLoadAligned(this);
	VectorRegister B = VectorLoadAligned(&Q);
	VectorStoreAligned(VectorAdd(A, B), this);
#else
	this->X += Q.X;
	this->Y += Q.Y;
	this->Z += Q.Z;
	this->W += Q.W;
#endif // PLATFORM_ENABLE_VECTORINTRINSICS

	DiagnosticCheckNaN();

	return *this;
}


inline RQuat RQuat::operator-(const RQuat& Q) const
{
#if PLATFORM_ENABLE_VECTORINTRINSICS
	VectorRegister A = VectorLoadAligned(this);
	VectorRegister B = VectorLoadAligned(&Q);
	return RQuat(VectorSubtract(A, B));
#else
	return RQuat(X - Q.X, Y - Q.Y, Z - Q.Z, W - Q.W);
#endif // PLATFORM_ENABLE_VECTORINTRINSICS
}


inline bool RQuat::Equals(const RQuat& Q, float Tolerance) const
{
#if PLATFORM_ENABLE_VECTORINTRINSICS
	const VectorRegister ToleranceV = VectorLoadFloat1(&Tolerance);
	const VectorRegister A = VectorLoadAligned(this);
	const VectorRegister B = VectorLoadAligned(&Q);

	const VectorRegister RotationSub = VectorAbs(VectorSubtract(A, B));
	const VectorRegister RotationAdd = VectorAbs(VectorAdd(A, B));
	return !VectorAnyGreaterThan(RotationSub, ToleranceV) || !VectorAnyGreaterThan(RotationAdd, ToleranceV);
#else
	return (RMath::Abs(X - Q.X) <= Tolerance && RMath::Abs(Y - Q.Y) <= Tolerance && RMath::Abs(Z - Q.Z) <= Tolerance && RMath::Abs(W - Q.W) <= Tolerance)
		|| (RMath::Abs(X + Q.X) <= Tolerance && RMath::Abs(Y + Q.Y) <= Tolerance && RMath::Abs(Z + Q.Z) <= Tolerance && RMath::Abs(W + Q.W) <= Tolerance);
#endif // PLATFORM_ENABLE_VECTORINTRINSICS
}

inline bool RQuat::IsIdentity(float Tolerance) const
{
	return Equals(RQuat::Identity, Tolerance);
}

inline RQuat RQuat::operator-=(const RQuat& Q)
{
#if PLATFORM_ENABLE_VECTORINTRINSICS
	VectorRegister A = VectorLoadAligned(this);
	VectorRegister B = VectorLoadAligned(&Q);
	VectorStoreAligned(VectorSubtract(A, B), this);
#else
	this->X -= Q.X;
	this->Y -= Q.Y;
	this->Z -= Q.Z;
	this->W -= Q.W;
#endif // PLATFORM_ENABLE_VECTORINTRINSICS

	DiagnosticCheckNaN();

	return *this;
}


inline RQuat RQuat::operator*(const RQuat& Q) const
{
	RQuat Result;
	VectorQuaternionMultiply(&Result, this, &Q);
	
	Result.DiagnosticCheckNaN();
	
	return Result;
}


inline RQuat RQuat::operator*=(const RQuat& Q)
{
	VectorRegister A = VectorLoadAligned(this);
	VectorRegister B = VectorLoadAligned(&Q);
	VectorRegister Result;
	VectorQuaternionMultiply(&Result, &A, &B);
	VectorStoreAligned(Result, this);

	DiagnosticCheckNaN();

	return *this; 
}


inline RQuat RQuat::operator*=(const float Scale)
{
#if PLATFORM_ENABLE_VECTORINTRINSICS
	VectorRegister A = VectorLoadAligned(this);
	VectorRegister B = VectorSetFloat1(Scale);
	VectorStoreAligned(VectorMultiply(A, B), this);
#else
	X *= Scale;
	Y *= Scale;
	Z *= Scale;
	W *= Scale;
#endif // PLATFORM_ENABLE_VECTORINTRINSICS

	DiagnosticCheckNaN();

	return *this;
}


inline RQuat RQuat::operator*(const float Scale) const
{
#if PLATFORM_ENABLE_VECTORINTRINSICS
	VectorRegister A = VectorLoadAligned(this);
	VectorRegister B = VectorSetFloat1(Scale);
	return RQuat(VectorMultiply(A, B));
#else
	return RQuat(Scale * X, Scale * Y, Scale * Z, Scale * W);
#endif // PLATFORM_ENABLE_VECTORINTRINSICS
}


inline RQuat RQuat::operator/=(const float Scale)
{
#if PLATFORM_ENABLE_VECTORINTRINSICS
	VectorRegister A = VectorLoadAligned(this);
	VectorRegister B = VectorSetFloat1(Scale);
	VectorStoreAligned(VectorDivide(A, B), this);
#else
	const float Recip = 1.0f / Scale;
	X *= Recip;
	Y *= Recip;
	Z *= Recip;
	W *= Recip;
#endif // PLATFORM_ENABLE_VECTORINTRINSICS

	DiagnosticCheckNaN();

	return *this;
}


inline RQuat RQuat::operator/(const float Scale) const
{
#if PLATFORM_ENABLE_VECTORINTRINSICS
	VectorRegister A = VectorLoadAligned(this);
	VectorRegister B = VectorSetFloat1(Scale);
	return RQuat(VectorDivide(A, B));
#else
	const float Recip = 1.0f / Scale;
	return RQuat(X * Recip, Y * Recip, Z * Recip, W * Recip);
#endif // PLATFORM_ENABLE_VECTORINTRINSICS
}

inline bool RQuat::Identical(const RQuat* Q, const int PortFlags) const
{
	return X == Q->X && Y == Q->Y && Z == Q->Z && W == Q->W;
}

inline bool RQuat::operator==(const RQuat& Q) const
{
#if PLATFORM_ENABLE_VECTORINTRINSICS
	const VectorRegister A = VectorLoadAligned(this);
	const VectorRegister B = VectorLoadAligned(&Q);
	return VectorMaskBits(VectorCompareEQ(A, B)) == 0x0F;
#else
	return X == Q.X && Y == Q.Y && Z == Q.Z && W == Q.W;
#endif // PLATFORM_ENABLE_VECTORINTRINSICS
}


inline bool RQuat::operator!=(const RQuat& Q) const
{
#if PLATFORM_ENABLE_VECTORINTRINSICS
	const VectorRegister A = VectorLoadAligned(this);
	const VectorRegister B = VectorLoadAligned(&Q);
	return VectorMaskBits(VectorCompareNE(A, B)) != 0x00;
#else
	return X != Q.X || Y != Q.Y || Z != Q.Z || W != Q.W;
#endif // PLATFORM_ENABLE_VECTORINTRINSICS
}


inline float RQuat::operator|(const RQuat& Q) const
{
	return X * Q.X + Y * Q.Y + Z * Q.Z + W * Q.W;
}


inline void RQuat::Normalize(float Tolerance)
{
#if PLATFORM_ENABLE_VECTORINTRINSICS
	const VectorRegister Vector = VectorLoadAligned(this);

	const VectorRegister SquareSum = VectorDot4(Vector, Vector);
	const VectorRegister NonZeroMask = VectorCompareGE(SquareSum, VectorLoadFloat1(&Tolerance));
	const VectorRegister InvLength = VectorReciprocalSqrtAccurate(SquareSum);
	const VectorRegister NormalizedVector = VectorMultiply(InvLength, Vector);
	VectorRegister Result = VectorSelect(NonZeroMask, NormalizedVector, GlobalVectorConstants::Float0001);

	VectorStoreAligned(Result, this);
#else
	const float SquareSum = X * X + Y * Y + Z * Z + W * W;

	if (SquareSum >= Tolerance)
	{
		const float Scale = RMath::InvSqrt(SquareSum);

		X *= Scale; 
		Y *= Scale; 
		Z *= Scale;
		W *= Scale;
	}
	else
	{
		*this = RQuat::Identity;
	}
#endif // PLATFORM_ENABLE_VECTORINTRINSICS
}


inline RQuat RQuat::GetNormalized(float Tolerance) const
{
	RQuat Result(*this);
	Result.Normalize(Tolerance);
	return Result;
}



inline bool RQuat::IsNormalized() const
{
#if PLATFORM_ENABLE_VECTORINTRINSICS
	VectorRegister A = VectorLoadAligned(this);
	VectorRegister TestValue = VectorAbs(VectorSubtract(VectorOne(), VectorDot4(A, A)));
	return !VectorAnyGreaterThan(TestValue, GlobalVectorConstants::ThreshQuatNormalized);
#else
	return (RMath::Abs(1.f - SizeSquared()) < THRESH_QUAT_NORMALIZED);
#endif // PLATFORM_ENABLE_VECTORINTRINSICS
}


inline float RQuat::Size() const
{
	return RMath::Sqrt(X * X + Y * Y + Z * Z + W * W);
}


inline float RQuat::SizeSquared() const
{
	return (X * X + Y * Y + Z * Z + W * W);
}

inline float RQuat::GetAngle() const
{
	return 2.f * RMath::Acos(W);
}


inline void RQuat::ToAxisAndAngle(Vector& Axis, float& Angle) const
{
	Angle = GetAngle();
	Axis = GetRotationAxis();
}

inline Vector RQuat::GetRotationAxis() const
{
#if PLATFORM_ENABLE_VECTORINTRINSICS
	Vector V;
	VectorRegister A = VectorLoadAligned(this);
	VectorRegister R = VectorNormalizeSafe(VectorSet_W0(A), GlobalVectorConstants::Float1000);
	VectorStoreFloat3(R, &V);
	return V;
#else
	const float SquareSum = X * X + Y * Y + Z * Z;
	if (SquareSum < SMALL_NUMBER)
	{
		return Vector::XAxisVector;
	}
	const float Scale = RMath::InvSqrt(SquareSum);
	return Vector(X * Scale, Y * Scale, Z * Scale);
#endif // PLATFORM_ENABLE_VECTORINTRINSICS
}

float RQuat::AngularDistance(const RQuat& Q) const
{
	float InnerProd = X*Q.X + Y*Q.Y + Z*Q.Z + W*Q.W;
	return RMath::Acos((2 * InnerProd * InnerProd) - 1.f);
}


inline Vector RQuat::RotateVector(Vector V) const
{	
	// http://people.csail.mit.edu/bkph/articles/Quaternions.pdf
	// V' = V + 2w(Q x V) + (2Q x (Q x V))
	// refactor:
	// V' = V + w(2(Q x V)) + (Q x (2(Q x V)))
	// T = 2(Q x V);
	// V' = V + w*(T) + (Q x T)

	const Vector Q(X, Y, Z);
	const Vector T = 2.f * Vector::CrossProduct(Q, V);
	const Vector Result = V + (W * T) + Vector::CrossProduct(Q, T);
	return Result;
}

inline Vector RQuat::UnrotateVector(Vector V) const
{	
	const Vector Q(-X, -Y, -Z); // Inverse
	const Vector T = 2.f * Vector::CrossProduct(Q, V);
	const Vector Result = V + (W * T) + Vector::CrossProduct(Q, T);
	return Result;
}


inline RQuat RQuat::Inverse() const
{
	checkSlow(IsNormalized());

#if PLATFORM_ENABLE_VECTORINTRINSICS
	return RQuat(VectorQuaternionInverse(VectorLoadAligned(this)));
#else
	return RQuat(-X, -Y, -Z, W);
#endif // PLATFORM_ENABLE_VECTORINTRINSICS
}


inline void RQuat::EnforceShortestArcWith(const RQuat& OtherQuat)
{
	const float DotResult = (OtherQuat | *this);
	const float Bias = RMath::FloatSelect(DotResult, 1.0f, -1.0f);

	X *= Bias;
	Y *= Bias;
	Z *= Bias;
	W *= Bias;
}

	
inline Vector RQuat::GetAxisX() const
{
	return RotateVector(Vector(1.f, 0.f, 0.f));
}


inline Vector RQuat::GetAxisY() const
{
	return RotateVector(Vector(0.f, 1.f, 0.f));
}


inline Vector RQuat::GetAxisZ() const
{
	return RotateVector(Vector(0.f, 0.f, 1.f));
}


inline Vector RQuat::GetForwardVector() const
{
	return GetAxisX();
}

inline Vector RQuat::GetRightVector() const
{
	return GetAxisY();
}

inline Vector RQuat::GetUpVector() const
{
	return GetAxisZ();
}

inline Vector RQuat::Vector() const
{
	return GetAxisX();
}


inline float RQuat::Error(const RQuat& Q1, const RQuat& Q2)
{
	const float cosom = RMath::Abs(Q1.X * Q2.X + Q1.Y * Q2.Y + Q1.Z * Q2.Z + Q1.W * Q2.W);
	return (RMath::Abs(cosom) < 0.9999999f) ? RMath::Acos(cosom)*(1.f / PI) : 0.0f;
}


inline float RQuat::ErrorAutoNormalize(const RQuat& A, const RQuat& B)
{
	RQuat Q1 = A;
	Q1.Normalize();

	RQuat Q2 = B;
	Q2.Normalize();

	return RQuat::Error(Q1, Q2);
}

/**
 * Fast Linear Quaternion Interpolation.
 * Result is NOT normalized.
 */
inline RQuat RQuat::FastLerp(const RQuat& A, const RQuat& B, const float Alpha)
{
	// To ensure the 'shortest route', we make sure the dot product between the both rotations is positive.
	const float DotResult = (A | B);
	const float Bias = RMath::FloatSelect(DotResult, 1.0f, -1.0f);
	return (B * Alpha) + (A * (Bias * (1.f - Alpha)));
}


inline RQuat RQuat::FastBilerp(const RQuat& P00, const RQuat& P10, const RQuat& P01, const RQuat& P11, float FracX, float FracY)
{
	return RQuat::FastLerp(
		RQuat::FastLerp(P00,P10,FracX),
		RQuat::FastLerp(P01,P11,FracX),
		FracY
	);
}


inline bool RQuat::ContainsNaN() const
{
	return (!RMath::IsFinite(X) ||
			!RMath::IsFinite(Y) ||
			!RMath::IsFinite(Z) ||
			!RMath::IsFinite(W)
	);
}


template<> struct TIsPODType<RQuat> { enum { Value = true }; };

/* RMath inline functions
 *****************************************************************************/

template<class U>
FORCEINLINE_DEBUGGABLE RQuat RMath::Lerp( const RQuat& A, const RQuat& B, const U& Alpha)
{
	return RQuat::Slerp(A, B, Alpha);
}

template<class U>
FORCEINLINE_DEBUGGABLE RQuat RMath::BiLerp(const RQuat& P00, const RQuat& P10, const RQuat& P01, const RQuat& P11, float FracX, float FracY)
{
	RQuat Result;

	Result = Lerp(
		RQuat::Slerp_NotNormalized(P00,P10,FracX),
		RQuat::Slerp_NotNormalized(P01,P11,FracX),
		FracY
		);

	return Result;
}

template<class U>
FORCEINLINE_DEBUGGABLE RQuat RMath::CubicInterp( const RQuat& P0, const RQuat& T0, const RQuat& P1, const RQuat& T1, const U& A)
{
	return RQuat::Squad(P0, T0, P1, T1, A);
}

