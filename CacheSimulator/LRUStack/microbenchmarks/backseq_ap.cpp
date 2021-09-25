/*
 * Iterate over an array of size len
 * access number of times from the end
 * of the array.
 */


#include <omp.h>
#include <string>
#include <cstdio>
#include <cstdlib>
#include <stdint.h>
#include <assert.h>


using namespace std;

void ParBase(int64_t len, int64_t accesses)
{
    char b;
    volatile char* a = (char*)malloc(len);
    int64_t i = len - 1;

    #pragma omp parallel for
    for (int64_t acc = 0; acc < accesses; acc++)
    {
        int64_t index = i;
        if (index < 0) index = index % (len - 1);
        b = a[0];

        i--;
    }
}

void Par(int64_t len, int64_t accesses)
{
    char b;
    volatile char* a = (char*)malloc(len);
    int64_t i = len - 1;

    #pragma omp parallel for
    for (int64_t acc = 0; acc < accesses; acc++)
    {
        int64_t index = i;
        if (index < 0) index = index % (len - 1);
        b = a[index];

        i--;
    }
}

void SeqBase(int64_t len, int64_t accesses)
{
    char b;
    volatile char* a = (char*)malloc(len);
    int64_t i = len - 1;

    for (int64_t acc = 0; acc < accesses; acc++)
    {
        int64_t index = i;
        if (index < 0) index = index % (len - 1);
        b = a[0];

        i--;
    }
}

void Seq(int64_t len, int64_t accesses)
{
    char b;
    volatile char* a = (char*)malloc(len);
    int64_t i = len - 1;

    for (int64_t acc = 0; acc < accesses; acc++)
    {
        int64_t index = i;
        if (index < 0) index = index % (len - 1);
        b = a[index];

        i--;
    }
}

