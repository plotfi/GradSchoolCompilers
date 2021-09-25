/* This is the main function file.
 * All other code links with this.
 * The main function takes a bunch
 * of parameters and calls the
 * proper microbenchmark function.
 */

#include <omp.h>
#include <string>
#include <cstdio>
#include <cstdlib>
#include <stdint.h>
#include <iostream>
#include <assert.h>


#ifdef _WIN32

#define atoll _atoi64

#else

#include <unistd.h>

#endif

using namespace std;

void ParBase(int64_t len, int64_t accesses);
void Par(int64_t len, int64_t accesses);
void SeqBase(int64_t len, int64_t accesses);
void Seq(int64_t len, int64_t accesses);

int main(int argc, char** argv)
{
    string   runtype;
    int64_t size;
    int64_t accesses;
    int64_t threads;
    int64_t dobase;


    if (argc >= 4)
    {
        runtype  = string(argv[1]);
        size     = atoll  (argv[2]);
        accesses = atoll  (argv[3]);
        threads  = argc < 5 ? 2 :
                   atoll  (argv[4]);
        dobase   = argc < 6 ? 0 :
                   atoll  (argv[5]);
    }
    else if(argc == 2)
    {
        runtype  = string(argv[1]);
        size     = 1073741824;
        accesses = 1000;
        threads  = 2;
        dobase   = 0;
    }
    else
    {
        goto end;
    }

    assert(size     > 0);
    assert(threads  > 0);
    assert(accesses > 0);
    assert(dobase  == 0 ||
            dobase == 1);

    if (runtype == "seq")
    {
        printf("Starting%sSequential Microbenchmark.", (dobase ? " Baseline " : " "));
        if (dobase) SeqBase(size, accesses);
        else        Seq(size, accesses);
    }
    else if (runtype == "par")
    {
        printf("Starting%sParallel Microbenchmark.", (dobase ? " Baseline " : " "));
        omp_set_num_threads(threads);
        if (dobase) ParBase(size, accesses);
        else        Par(size, accesses);
    }
    else
    {
        goto end;
    }

    char dumb;
    if (argc >= 7 && string(argv[6]) == "-debug")
        cin >> dumb;

    printf("\n%s microbenchmark complete.\n", argv[0]);
    return 0;

end:
    printf("%s <runtype=seq|par> <size > 0> <accesses > 0> <threads > 1> <dobase=0|1>  \n", argv[0]);
    return 0;
}

