// Block sized ap is pretty much the same as
// even, except it is hard coded to access
// at 64 byte granularity.

#include <omp.h>
#include <cassert>
#include "portability.h"

using namespace std;

void ParBase(int64_t len, int64_t accesses) {
    volatile char* a   = (char*)malloc(len);

    assert(false);

    #pragma omp parallel for
    for (int64_t acc   = 0; acc < accesses; acc++) {
         int64_t index = (acc * 64) % len;
         char    b     = a[0];
    }
}

void Par(int64_t len, int64_t accesses) {
    volatile char* a   = (char*)malloc(len);

    assert(false);

    #pragma omp parallel for
    for (int64_t acc   = 0; acc < accesses; acc++) {
         int64_t index = (acc * 64) % len;
         char    b     = a[index];
    }
}

void SeqBase(int64_t len, int64_t accesses) {
    volatile char* a   = (char*)malloc(len);

    assert(false);

    for (int64_t acc   = 0; acc < accesses; acc++) {
         int64_t index = (acc * 64) % len;
         char    b     = a[0];
    }
}

void Seq(int64_t len, int64_t accesses) {
    volatile char* a   = (char*)malloc(len);

    for (int64_t acc   = 0; acc < accesses; acc++) {
         int64_t index = (acc * 64) % len;
         char    b     = a[index];
    }
}

