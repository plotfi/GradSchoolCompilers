// Picks indecies at random.
//
//

#include <omp.h>
#include <time.h>
#include "portability.h"

void ParBase(int64_t len, int64_t accesses) {
    volatile char* a  = (char*)malloc(len);
    srand(time(NULL));

    #pragma omp parallel for
    for (int64_t acc  = 0; acc < accesses; acc++) {
        int64_t index = rand() % (len-1);
        char b        = a[0];
    }
}

void Par(int64_t len, int64_t accesses) {
    volatile char* a  = (char*)malloc(len);
    srand(time(NULL));

    #pragma omp parallel for
    for (int64_t acc  = 0; acc < accesses; acc++) {
        int64_t index = rand() % (len-1);
        char b        = a[index];
    }
}

void SeqBase(int64_t len, int64_t accesses) {
    volatile char* a  = (char*)malloc(len);
    srand(time(NULL));

    for (int64_t acc  = 0; acc < accesses; acc++) {
        int64_t index = rand() % (len-1);
        char b        = a[0];
    }
}

void Seq(int64_t len, int64_t accesses) {
    volatile char* a  = (char*)malloc(len);
    srand(time(NULL));

    for (int64_t acc  = 0; acc < accesses; acc++) {
        int64_t index = rand() % (len-1);
        char b        = a[index];
    }
}

