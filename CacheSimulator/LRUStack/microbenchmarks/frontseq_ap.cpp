// This access pattern is supposed to just
// access accesses locations of a len size
// array from the start of the array.

#include <omp.h>
#include "portability.h"

void ParBase(int64_t len, int64_t accesses) {
    volatile char* a   = (char*)malloc(len);

    #pragma omp parallel for
    for (int64_t acc   = 0; acc < accesses; acc++) {
         int64_t index = acc % (len-1);
         char    b     = a[0];
    }
}

void Par(int64_t len, int64_t accesses) {
    volatile char* a   = (char*)malloc(len);

    #pragma omp parallel for
    for (int64_t acc   = 0; acc < accesses; acc++) {
         int64_t index = acc % (len-1);
         char    b     = a[index];
    }
}

void SeqBase(int64_t len, int64_t accesses) {
    volatile char* a   = (char*)malloc(len);

    for (int64_t acc   = 0; acc < accesses; acc++) {
         int64_t index = acc % (len-1);
         char    b     = a[0];
    }
}

void Seq(int64_t len, int64_t accesses) {
    volatile char* a   = (char*)malloc(len);

    for (int64_t acc   = 0; acc < accesses; acc++) {
         int64_t index = acc % (len-1);
         char    b     = a[index];
    }
}

