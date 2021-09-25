// The even access pattern evenly
// distributes accesses accesses
// accross a len sized array.
#include <iostream>
#include <omp.h>
#include <assert.h>
#include "portability.h"

void ParBase(int64_t len, int64_t accesses) {
    volatile char* a   = (char*)malloc(len); assert(a);

    #pragma omp parallel for
    for (int64_t acc   = 0; acc < accesses; acc++) {
         int64_t index = (acc * (len/accesses)) % len;
         char b        = a[0];
    }
}

void Par(int64_t len, int64_t accesses) {
    volatile char* a   = (char*)malloc(len); assert(a);

    #pragma omp parallel for
    for (int64_t acc   = 0; acc < accesses; acc++) {
         int64_t index = (acc * (len/accesses)) % len;
         char b        = a[index];
    }
}

void SeqBase(int64_t len, int64_t accesses) {
    volatile char* a   = (char*)malloc(len); assert(a);

    for (int64_t acc   = 0; acc < accesses; acc++) {
         int64_t index = (acc * (len/accesses)) % len;
         char b        = a[0];
    }
}

void Seq(int64_t len, int64_t accesses) {
    volatile char* a   = (char*)malloc(len); assert(a);

    for (int64_t acc   = 0; acc < accesses; acc++) {
         int64_t index = (acc * (len/accesses)) % len;
         char b        = a[index];
    }
}

