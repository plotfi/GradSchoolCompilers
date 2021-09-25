#include <stdio.h>

int main()
{
    int i = 0;

    printf("basic_do test\n");

    do {
        printf("Indunction: %d\n", i);

        do {
            printf("Indunction(emb): %d\n", i);
        } while(i);

        i++;
    } while (i>1);

    do {

        int j = 0;

        printf("Indunction(i): %d\n", i);

        do {
            printf("Indunction(j): %d\n", j);
            j++;
        } while(j<5);

        i++;
    } while (i<10);
    
    return 0;
}

