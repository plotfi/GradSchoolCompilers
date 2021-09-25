#include <stdio.h>

int main()
{
    int i = 0;

    for (i=0; i< 12; i++)
    {
        if (i %2) continue;
        else printf("foo\n");
    }

    return 0;
}

