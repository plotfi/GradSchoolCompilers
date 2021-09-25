
#include <stdio.h>
#include <setjmp.h>
static jmp_buf env;


void foo()
{
    int i = 0;
    for(i=0;i<20;i++) {
        if (i==19)
    longjmp(env, -1);
    }
}

int main()
{
    int i = 0;

    setjmp(env);


    printf("i is %d\n", i);

    if (i < 20)
        i++;
    else
        goto end;


    foo();


end:

    return 0;
}



