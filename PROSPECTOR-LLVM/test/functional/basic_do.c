#include <stdio.h>

int main()
{
    int i = 0;

    printf("basic_do test\n");

    do {
        printf("Indunction: %d\n", i);
        i++;
    } while (i>1);

    do {
        printf("Indunction: %d\n", i);
        i++;
    } while (i<10);
    
    return 0;
}

