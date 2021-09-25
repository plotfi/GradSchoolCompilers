#include <stdio.h>

int main()
{
    int i = 0;
    int j = 0;

    printf("basic_gotoloop test\n");

    if (i<10) goto loop;
    else goto end;

loop:

    printf("Indunction(i): %d\n", i);
    i++;

embloop:

    printf("Indunction(j): %d\n", j);
    j++;

    if (j<10) goto embloop;

embend:


    if (i<10) goto loop;

end:

    return 0;

}

