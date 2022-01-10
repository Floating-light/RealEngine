#pragma once 

#include <emmintrin.h>

#define VectorReplicate(Vec, Index) _mm_shuffle_ps(Vec, Vec, _MM_SHUFFLE(Index, Index, Index, Index) )

namespace RealSSE
{
    // return a vector4(Vec[Index], Vec[Index], Vec[Index], Vec[Index]) 
    // inline __m128 VectorReplicate(__m128 Vec, int Index)
    // {
    //     return _mm_shuffle_ps(Vec, Vec, _MM_SHUFFLE(Index, Index, Index, Index) );
    // }

    inline void MatrixMultiply(void* Result, const void* Mat1, const void* Mat2)
    {
        const __m128 *A = (const __m128*) Mat1;
        const __m128 *B = (const __m128*) Mat2;
        __m128 * R = (__m128*) Result;
        __m128 Temp, R0, R1, R2, R3;

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
