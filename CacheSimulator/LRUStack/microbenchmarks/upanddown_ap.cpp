// The even access pattern evenly
// distributes accesses accesses
// accross a len sized array.
#include <iostream>
#include <omp.h>
#include <assert.h>
#include "portability.h"

void ParBase(int64_t len, int64_t accesses) {
    volatile char* a   = (char*)malloc(len); assert(a);

    assert(false);

    assert(len >= 64);

    int64_t increment = 1;
    int64_t i = 0;

    int tid = omp_get_thread_num();

    #pragma omp parallel for
    for (int64_t acc = 0; acc < accesses; acc++)
    {
        if (i*64 >= len)
        {
            i = (len/64) - 1;
            increment = -increment;
        }
        else if (i*64 < 0)
        {
            i = 0;
            increment = -increment;
        }

        int64_t index = i * 64;

//        cout << "index: " << index << " i: " << i << " acc: " << acc << " threadid: " << tid << endl;
        char b = a[0];

        i+=increment;
    }
}

void Par(int64_t len, int64_t accesses) {
    volatile char* a   = (char*)malloc(len); assert(a);

    assert(len >= 64);

    int64_t increment = 1;
    int64_t i = 0;

    int tid = omp_get_thread_num();

    #pragma omp parallel for
    for (int64_t acc = 0; acc < accesses; acc++)
    {
        if (i*64 >= len)
        {
            i = (len/64) - 1;
            increment = -increment;
        }
        else if (i*64 < 0)
        {
            i = 0;
            increment = -increment;
        }

        int64_t index = i * 64;

 //       cout << "index: " << index << " i: " << i << " acc: " << acc << " threadid: " << tid << endl;
        char b = a[index];

        i+=increment;
    }
}

void SeqBase(int64_t len, int64_t accesses) {
    volatile char* a   = (char*)malloc(len); assert(a);

    assert(len >= 64);

    int64_t increment = 1;
    int64_t i = 0;

    for (int64_t acc = 0; acc < accesses; acc++)
    {
        if (i*64 >= len)
        {
            i = (len/64) - 1;
            increment = -increment;
        }
        else if (i*64 < 0)
        {
            i = 0;
            increment = -increment;
        }

        int64_t index = i * 64;

        //cout << "index: " << index << " i: " << i << " acc: " << acc << endl;
        char b = a[0];

        i+=increment;
    }
}

void Seq(int64_t len, int64_t accesses) {
    volatile char* a   = (char*)malloc(len); assert(a);

    assert(len >= 64);

    int64_t increment = 1;
    int64_t i = 0;

    for (int64_t acc = 0; acc < accesses; acc++)
    {
        if (i*64 >= len)
        {
            i = (len/64) - 1;
            increment = -increment;
        }
        else if (i*64 < 0)
        {
            i = 0;
            increment = -increment;
        }

        int64_t index = i * 64;

        //cout << "index: " << index << " i: " << i << " acc: " << acc << endl;
        char b = a[index];

        i+=increment;
    }
}

