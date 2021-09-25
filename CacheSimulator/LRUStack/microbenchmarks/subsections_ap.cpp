// Iterates over the array multiple
// subsections of the array many times
// for each subsection access number of times.

#include <omp.h>
#include "portability.h"

void ParBase(int64_t len, int64_t accesses) {
    volatile char* a   = (char*)malloc(len);

    #pragma omp parallel for
    for (int i = 0; i < len; i+=(len/4)){
        for (int64_t acc = 0; acc < accesses/4; acc++) {
            int64_t index = (i + (acc % 4)) % (len-1);
            char b = a[0];
        }
    }
}

void Par(int64_t len, int64_t accesses) {
    volatile char* a   = (char*)malloc(len);

    #pragma omp parallel for
    for (int i = 0; i < len; i+=(len/4)){
        for (int64_t acc = 0; acc < accesses/4; acc++) {
            int64_t index = (i + (acc % 4)) % (len-1);
            char b = a[index];
        }
    }
}

void SeqBase(int64_t len, int64_t accesses) {
    volatile char* a   = (char*)malloc(len);

    for (int i = 0; i < len; i+=(len/4)){
        for (int64_t acc = 0; acc < accesses/4; acc++) {
            int64_t index = (i + (acc % 4)) % (len-1);
            char b = a[0];
        }
    }
}

void Seq(int64_t len, int64_t accesses) {
    volatile char* a   = (char*)malloc(len);

    for (int i = 0; i < len; i+=(len/4)){
        for (int64_t acc = 0; acc < accesses/4; acc++) {
            int64_t index = (i + (acc % 4)) % (len-1);
            char b = a[index];
        }
    }
}

