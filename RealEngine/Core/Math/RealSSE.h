#pragma once 

#include <emmintrin.h>

using VectorRegister = __m128;
// Replicates on element into all four elements
#define VectorReplicate(Vec, Index) _mm_shuffle_ps(Vec, Vec, _MM_SHUFFLE(Index, Index, Index, Index) )
// return a VectorRegister with w = 0, xyz using ptr.
#define VectorLoadFloat3_W0(Ptr) _mm_setr_ps(((const float *)Ptr)[0], ((const float *)Ptr)[1], ((const float *)Ptr)[2], 0)

#define SHUFFLEMASK(A0,A1,B2,B3) ( (A0) | ((A1)<<2) | ((B2)<<4) | ((B3)<<6) )

#define VectorSwizzle( Vec, X, Y, Z, W )	_mm_shuffle_ps( Vec, Vec, SHUFFLEMASK(X,Y,Z,W) )
#define VectorStoreAligned( Vec, Ptr )	_mm_store_ps( (float*)(Ptr), Vec )
inline VectorRegister MakeVectorRegister( float X, float Y, float Z, float W )
{
	return _mm_setr_ps( X, Y, Z, W );
}

static const VectorRegister QMULTI_SIGN_MASK0 = MakeVectorRegister( 1.f, -1.f, 1.f, -1.f );
static const VectorRegister QMULTI_SIGN_MASK1 = MakeVectorRegister( 1.f, 1.f, -1.f, -1.f );
static const VectorRegister QMULTI_SIGN_MASK2 = MakeVectorRegister( -1.f, 1.f, 1.f, -1.f );

inline VectorRegister VectorAdd( const VectorRegister& Vec1, const VectorRegister& Vec2 )
{
	return _mm_add_ps(Vec1, Vec2);
}
// @return		VectorRegister( Vec1.x*Vec2.x, Vec1.y*Vec2.y, Vec1.z*Vec2.z, Vec1.w*Vec2.w )
inline VectorRegister VectorMultiply( const VectorRegister& Vec1, const VectorRegister& Vec2 )
{
	return _mm_mul_ps(Vec1, Vec2);
}

// @return		VectorRegister( Vec1.x*Vec2.x + Vec3.x, Vec1.y*Vec2.y + Vec3.y, Vec1.z*Vec2.z + Vec3.z, Vec1.w*Vec2.w + Vec3.w )
inline VectorRegister VectorMultiplyAdd(const VectorRegister& Vec1, const VectorRegister& Vec2, const VectorRegister& Vec3)
{
#if PLATFORM_ALWAYS_HAS_FMA3
	return _mm_fmadd_ps(Vec1, Vec2, Vec3);
#else
	return _mm_add_ps(_mm_mul_ps(Vec1, Vec2), Vec3);
#endif
}

//  @return		VectorRegister( Vec3.x - Vec1.x*Vec2.x, Vec3.y - Vec1.y*Vec2.y, Vec3.z - Vec1.z*Vec2.z, Vec3.w - Vec1.w*Vec2.w )
inline VectorRegister VectorNegateMultiplyAdd(VectorRegister Vec1, VectorRegister Vec2, VectorRegister Vec3)
{
#if PLATFORM_ALWAYS_HAS_FMA3
	return _mm_fnmadd_ps(Vec1, Vec2, Vec3);
#else
	return _mm_sub_ps(Vec3, _mm_mul_ps(Vec1, Vec2));
#endif
}
inline VectorRegister VectorQuaternionMultiply2( const VectorRegister& Quat1, const VectorRegister& Quat2 )
{
	VectorRegister Result = VectorMultiply(VectorReplicate(Quat1, 3), Quat2);
	Result = VectorMultiplyAdd( VectorMultiply(VectorReplicate(Quat1, 0), VectorSwizzle(Quat2, 3,2,1,0)), QMULTI_SIGN_MASK0, Result);
	Result = VectorMultiplyAdd( VectorMultiply(VectorReplicate(Quat1, 1), VectorSwizzle(Quat2, 2,3,0,1)), QMULTI_SIGN_MASK1, Result);
	Result = VectorMultiplyAdd( VectorMultiply(VectorReplicate(Quat1, 2), VectorSwizzle(Quat2, 1,0,3,2)), QMULTI_SIGN_MASK2, Result);

	return Result;
}

inline void VectorQuaternionMultiply( void* __restrict Result, const void* __restrict Quat1, const void* __restrict Quat2)
{
	*((VectorRegister *)Result) = VectorQuaternionMultiply2(*((const VectorRegister *)Quat1), *((const VectorRegister *)Quat2));
}

inline VectorRegister VectorCross( const VectorRegister& Vec1, const VectorRegister& Vec2 )
{
	VectorRegister C = VectorMultiply(Vec1, _mm_shuffle_ps(Vec2, Vec2, SHUFFLEMASK(1, 2, 0, 3)));
	C = VectorNegateMultiplyAdd(_mm_shuffle_ps(Vec1, Vec1, SHUFFLEMASK(1, 2, 0, 3)), Vec2, C);
	C = _mm_shuffle_ps(C, C, SHUFFLEMASK(1, 2, 0, 3));
	return C;
}
namespace RealSSE
{
    // return a vector4(Vec[Index], Vec[Index], Vec[Index], Vec[Index]) 
    // inline __m128 VectorReplicate(__m128 Vec, int Index)
    // {
    //     return _mm_shuffle_ps(Vec, Vec, _MM_SHUFFLE(Index, Index, Index, Index) );
    // }

    inline void MatrixMultiply(void* Result, const void* Mat1, const void* Mat2)
    {
        const VectorRegister *A = (const VectorRegister*) Mat1;
        const VectorRegister *B = (const VectorRegister*) Mat2;
        VectorRegister * R = (VectorRegister*) Result;
        VectorRegister Temp, R0, R1, R2, R3;

        Temp = _mm_mul_ps(           VectorReplicate(A[0], 0), B[0]);
        Temp = _mm_add_ps(_mm_mul_ps(VectorReplicate(A[0], 1), B[1]), Temp);
        Temp = _mm_add_ps(_mm_mul_ps(VectorReplicate(A[0], 2), B[2]), Temp);
        R0   = _mm_add_ps(_mm_mul_ps(VectorReplicate(A[0], 3), B[3]), Temp);

        Temp = _mm_mul_ps(           VectorReplicate(A[1], 0), B[0]);
        Temp = _mm_add_ps(_mm_mul_ps(VectorReplicate(A[1], 1), B[1]), Temp);
        Temp = _mm_add_ps(_mm_mul_ps(VectorReplicate(A[1], 2), B[2]), Temp);
        R1   = _mm_add_ps(_mm_mul_ps(VectorReplicate(A[1], 3), B[3]), Temp);

        Temp = _mm_mul_ps(           VectorReplicate(A[2], 0), B[0]);
        Temp = _mm_add_ps(_mm_mul_ps(VectorReplicate(A[2], 1), B[1]), Temp);
        Temp = _mm_add_ps(_mm_mul_ps(VectorReplicate(A[2], 2), B[2]), Temp);
        R2   = _mm_add_ps(_mm_mul_ps(VectorReplicate(A[2], 3), B[3]), Temp);

        Temp = _mm_mul_ps(           VectorReplicate(A[3], 0), B[0]);
        Temp = _mm_add_ps(_mm_mul_ps(VectorReplicate(A[3], 1), B[1]), Temp);
        Temp = _mm_add_ps(_mm_mul_ps(VectorReplicate(A[3], 2), B[2]), Temp);
        R3   = _mm_add_ps(_mm_mul_ps(VectorReplicate(A[3], 3), B[3]), Temp);

        R[0] = R0;
        R[1] = R1;
        R[2] = R2;
        R[3] = R3;
    }
} // namespace RealSSE

/**
 * Rotate a vector using a unit Quaternion.
 *
 * @param Quat Unit Quaternion to use for rotation.
 * @param VectorW0 Vector to rotate. W component must be zero.
 * @return Vector after rotation by Quat.
 */
inline VectorRegister VectorQuaternionRotateVector(const VectorRegister& Quat, const VectorRegister& VectorW0)
{
	// Q * V * Q.Inverse
	//const VectorRegister InverseRotation = VectorQuaternionInverse(Quat);
	//const VectorRegister Temp = VectorQuaternionMultiply2(Quat, VectorW0);
	//const VectorRegister Rotated = VectorQuaternionMultiply2(Temp, InverseRotation);

	// Equivalence of above can be shown to be:
	// http://people.csail.mit.edu/bkph/articles/Quaternions.pdf
	// V' = V + 2w(Q x V) + (2Q x (Q x V))
	// refactor:
	// V' = V + w(2(Q x V)) + (Q x (2(Q x V)))
	// T = 2(Q x V);
	// V' = V + w*(T) + (Q x T)

	const VectorRegister QW = VectorReplicate(Quat, 3);
	VectorRegister T = VectorCross(Quat, VectorW0);
	T = VectorAdd(T, T);
	const VectorRegister VTemp0 = VectorMultiplyAdd(QW, T, VectorW0);
	const VectorRegister VTemp1 = VectorCross(Quat, T);
	const VectorRegister Rotated = VectorAdd(VTemp0, VTemp1);
	return Rotated;
}