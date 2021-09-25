#include <stdio.h>

int main()
{
    int i = 0;

    printf("basic_for exitbreak test\n");

    for (i=0; i<10; i++)
    {
        if (i > 3 && i%2)
            break;

        printf("Indunction: %d\n", i);
    }

    return 0;
}

