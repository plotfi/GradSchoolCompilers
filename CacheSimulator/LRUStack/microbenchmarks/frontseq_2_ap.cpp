// Iterates over the array multiple
// times. Multiple threads are used
// for different ranges of accesses.

#include <omp.h>
#include "portability.h"

void ParBase(int64_t len, int64_t accesses) {
    volatile char* a   = (char*)malloc(len);

    #pragma omp parallel for
    for (int64_t acc = 0; acc < accesses; acc+=len) {
        for (int i = 0; i < len; i++) {
            int64_t index = i;
            char b = a[0];
        }
    }
}

void Par(int64_t len, int64_t accesses) {
    volatile char* a   = (char*)malloc(len);

    #pragma omp parallel for
    for (int64_t acc = 0; acc < accesses; acc+=len) {
        for (int i = 0; i < len; i++) {
            int64_t index = i;
            char b = a[index];
        }
    }
}

void SeqBase(int64_t len, int64_t accesses) {
    volatile char* a   = (char*)malloc(len);

    for (int64_t acc = 0; acc < accesses; acc+=len) {
        for (int i = 0; i < len; i++) {
            int64_t index = i;
            char b = a[0];
        }
    }
}

void Seq(int64_t len, int64_t accesses) {
    volatile char* a   = (char*)malloc(len);

    for (int64_t acc = 0; acc < accesses; acc+=len) {
        for (int i = 0; i < len; i++) {
            int64_t index = i;
            char b = a[index];
        }
    }
}

