#include <stdio.h>
#include <omp.h>

int main() {

    omp_set_num_threads(2);

    #pragma omp parallel for
    for (int i = 0; i < 10; i++)
    {
        printf("index is: %d\n", i);
    }

    return 0;
}

