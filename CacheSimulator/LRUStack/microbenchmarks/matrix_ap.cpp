/* This access pattern is of tiling
 * as found in tiled matrix multiplication.
 * It should have good cache locality.
 */

/// Note: Currently these tiled matrix multiplication benchmarks
///       right now do not pay attention to the access count.
///

#include <omp.h>
#include <string>
#include <time.h>
#include <assert.h>

#include "portability.h"

// 64 bytes = 16 floats
#define BLOCK_SIZE 16

void ParBase(int64_t len, int64_t accesses)
{
    volatile float* A = (float*)malloc(sizeof(float)* len * len);
    volatile float* B = (float*)malloc(sizeof(float)* len * len);
    volatile float* C = (float*)malloc(sizeof(float)* len * len);

    assert(A);
    assert(B);
    assert(C);

    #pragma omp parallel for
    for (int64_t j = 0; j < len; ++j)
    {
        for (int64_t i = 0; i < len; ++i)
        {
            double sum = 0;
            for (int64_t k = 0; k < len; ++k)
            {
                int64_t ai = i * len + k;
                int64_t bi = k * len + j;
                double a = A[0];
                double b = B[0];
                sum += a * b;
            }
            int64_t ci = i * len + j;
            C[0] = (float)sum;
        }
    }
}

void Par(int64_t len, int64_t accesses)
{
    volatile float* A = (float*)malloc(sizeof(float)* len * len);
    volatile float* B = (float*)malloc(sizeof(float)* len * len);
    volatile float* C = (float*)malloc(sizeof(float)* len * len);

    assert(A);
    assert(B);
    assert(C);

    #pragma omp parallel for
    for (int64_t j = 0; j < len; ++j)
    {
        for (int64_t i = 0; i < len; ++i)
        {
            double sum = 0;
            for (int64_t k = 0; k < len; ++k)
            {
                int64_t ai = i * len + k;
                int64_t bi = k * len + j;
                double a = A[ai];
                double b = B[bi];
                sum += a * b;
            }
            int64_t ci = i * len + j;
            C[ci] = (float)sum;
        }
    }
}

void SeqBase(int64_t len, int64_t accesses)
{
    volatile float* A = (float*)malloc(sizeof(float)* len * len);
    volatile float* B = (float*)malloc(sizeof(float)* len * len);
    volatile float* C = (float*)malloc(sizeof(float)* len * len);

    assert(A);
    assert(B);
    assert(C);

    for (int64_t j = 0; j < len; ++j)
    {
        for (int64_t i = 0; i < len; ++i)
        {
            double sum = 0;
            for (int64_t k = 0; k < len; ++k)
            {
                int64_t ai = i * len + k;
                int64_t bi = k * len + j;
                double a = A[0];
                double b = B[0];
                sum += a * b;
            }
            int64_t ci = i * len + j;
            C[0] = (float)sum;
        }
    }
}

void Seq(int64_t len, int64_t accesses)
{
    volatile float* a = (float*)malloc(sizeof(float)* len * len);
    volatile float* b = (float*)malloc(sizeof(float)* len * len);
    volatile float* c = (float*)malloc(sizeof(float)* len * len);

    assert(A);
    assert(B);
    assert(C);

    for (i = 0; i < n; i += 2)
    {
        c[i] = 0;
        for (j = 0; j < n; j += 2)
        {
            for (x = i; x < min(i + 2, n); x++)
            {
                for (y = j; y < min(j + 2, n); y++)
                {
                    c[x] = c[x] + a[x][y] * b[y];
                }
            }
        }
    }

}

