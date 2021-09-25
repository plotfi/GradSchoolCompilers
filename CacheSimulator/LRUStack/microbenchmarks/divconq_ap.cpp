/* The divide and conquer access
 * pattern is not done yet, but the
 * idea is to access every sub array
 * in the middle before proceeding to
 * the other left and right sub array
 * much like divide and conquer algos
 * like merge sort.
 */

#include <omp.h>
#include <string>
#include <list>
#include <assert.h>

#include "portability.h"


void ParBase(int64_t len, int64_t accesses)
{
    //#pragma omp parallel for

    // Put your parallel baseline code here:
}

void Par(int64_t len, int64_t accesses)
{
    //#pragma omp parallel for

    // Put your parallel code here:
}

void SeqBase(int64_t len, int64_t accesses)
{
    // Put your sequential baseline code here:
}

void Seq(int64_t len, int64_t accesses)
{
    // Put your sequential code here:

    char* array = new char[len];
    volatile char* a = array;

    list<volatile char*> locations;
    list<int>   lenghtCounts;
    list<int>   accessCounts;

    locations.push_front(a);
    lenghtCounts.push_front(len);
    accessCounts.push_front(accesses);

    for (int64_t acc = 0; acc < accesses; )
    {
        a         = locations.front();
        len       = lenghtCounts.front();
        accesses  = accessCounts.front();

        locations.pop_front();
        lenghtCounts.pop_front();
        accessCounts.pop_front();

        int64_t i = len/2;
        char c = a[i];
        acc++;

        volatile char* b = &a[(len/2) + 1];
        locations.push_front(b);
        i = (len/2) + (len%2);
        lenghtCounts.push_front(i);
        accessCounts.push_front(accesses);
        acc++;

        locations.push_front(a);
        lenghtCounts.push_front(len/2);
        accessCounts.push_front(accesses);
    }
}

