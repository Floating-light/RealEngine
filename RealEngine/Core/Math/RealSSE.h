#pragma once 

#include <emmintrin.h>

namespace RealSSE
{
    // return a vector4(Vec[Index], Vec[Index], Vec[Index], Vec[Index]) 
    inline __m128 VectorReplicate(__m128 Vec, int Index)
    {
        return _mm_shuffle_ps(Vec, Vec, _MM_SHUFFLE(Index, Index, Index, Index));
    }

    inline void MatrixMultiply(void* Result, const void* Mat1, const void* Mat2)
    {
        const __m128 *A = (const __m128*) Mat1;
        const __m128 *B = (const __m128*) Mat2;
        __m128 * R = (__m128*) Result;
        __m128 Temp, R0, R1, R2, R3;

        Temp = _mm_mul_ps(VectorReplicate(A[0], 0) , B[1]);
        

    }
} // namespace RealSSE
