#include <omp.h>
#include <string>
#include <cstdio>
#include <cstdlib>
#include <assert.h>
#include <unistd.h>

using namespace std;

void ParBase(int64_t len, int64_t accesses)
{
    // Put your parallel baseline code here:

    #pragma omp parallel for
}

void Par(int64_t len, int64_t accesses)
{
    // Put your parallel code here:

    #pragma omp parallel for
}

void SeqBase(int64_t len, int64_t accesses)
{
    // Put your sequential baseline code here:
}

void Seq(int64_t len, int64_t accesses)
{
    // Put your sequential code here:
}

