#include <stdio.h>

void bar()
{

    int i = 2;
    int  k, h, j, l;

    printf("normal loop exit \n");
    while(i)
    {
        --i;
    }

    i = 100;

    while(k)
    {
    printf("break\n");
        break;
    }

    /*
    printf("continue...\n ");
    while(i)
    {
        --i;
        continue;
    }*/

    while(h)
    {
    printf("goto\n");
        goto label1;

    }

label1:




    while(j)
    {
    printf("return\n");
        return;
    }




}


int main()
{

    bar();

    return 0;
}
