#include <stdio.h>

int main()
{
    int i = 0;

    printf("basic_for test\n");

    for (i=0; i<10; i++)
    {
        int j = 0;

        printf("Indunction(i): %d\n", i);

        for (j=0; j<10; j++)
        {
            printf("Indunction(j): %d\n", j);
        }
    }

    return 0;
}

