#include <unistd.h>
#include <stdio.h>

void foo()//(int count)
{
    /*
    printf("count: %d\n", count);
    if (count < 0) return;

    for (int i = 0; i < 1; i++)
        foo(count - 1);
    */

    for (int i = 0; i < 6; i++)
        for (int j = 0; j < 3; j++)
            for (int h = 0; h < 5; h++)
            {
                if (i > 0) return;
            }
}

int main()
{
    foo();
    return 0;
}
