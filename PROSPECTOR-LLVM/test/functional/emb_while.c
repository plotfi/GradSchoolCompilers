#include <stdio.h>

int main()
{
    int i = 0;

    printf("emb_while test\n");

    while (i<10)
    {
        int j = 0;

        printf("Indunction(i): %d\n", i);

        while (j<5)
        {
            printf("Indunction(j): %d\n", j);
            j++;
        }

        i++;
    }
    
    return 0;
}

