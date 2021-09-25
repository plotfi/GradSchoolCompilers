#include <stdio.h>

int main()
{
    int i = 0;

    printf("basic_while exitbreak test\n");

    while (i<10)
    {
        printf("Indunction: %d\n", i);
        i++;

        if (i > 3 && i%2)
            break;
    }
    
    return 0;
}

