#include <stdio.h>

int main()
{
    int i = 0;

    printf("emb_while test\n");

    while (i<10)
    {
        int j = 0;

        printf("Indunction(i): %d\n", i);

        if (i > 8 && i%2)
            break;

        while (j<5)
        {
            printf("Indunction(j): %d\n", j);
            j++;

            if (i > 3 && i%2)
                break;

            if (j > 3 && i%3)
                break;
        }

        i++;
    }
    
    return 0;
}

