#include <stdio.h>

void foo()
{
int i,j,k,l,m,n;
    for(i=0;i<5;i++)
        for(j=0;j<2;j++)
        for(k=0;k<2;k++)
        for(l=0;l<2;l++)
        for(m=0;m<2;m++)
        for(n=0;n<2;n++)
            printf("Inner loop\n");


}

int main()
{
    int i=10;

    foo();
    

goto whileloop;
goto whileloop;
gwb:
goto forloop;
goto forloop;
gfl:
goto whileloop2;
goto whileloop2;
gw2:
goto dooloop;
goto dooloop;
gdl:

goto label;


whileloop:

    while (i)
    {
    //    printf("this is a while loop\n");
        --i;
    }

    goto gwb;


forloop:
    
    for(i=0;i<5;i++)
    {
   //     printf("this is a for loop\n");
    }


    goto gfl;

whileloop2:

    while (i)
    {
        //printf("this is a while loop\n");
        --i;
    }

    goto gw2;

dooloop:

    i = 2;
    do
    {
       // printf("this is a do loop\n");
    } while (--i);

    if (i) goto end;
    else goto label1;

    
    goto gdl;


label:


//    printf("this is a goto after label\n");


    i = 1;

    if (i) goto end;

label1:

 //   printf("this is a goto after label1\n");
    goto label;

end:

  //  printf("this is a goto after end\n");

    return 0;
}

