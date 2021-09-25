#include <stdio.h>

int main()
{
    int i = 0;

    printf("basic_gotoloop test\n");

    if (i>10) goto loop;
    else goto end;

loop:

    printf("Indunction: %d\n", i);
    i++;

    if (i>10) goto loop;

end:

    return 0;

}

