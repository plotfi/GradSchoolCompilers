

int main()
{
    int opcode = 0;
    int i = 0;

    static const void *codetable[] =
    {
        &&LABEL1
    };


    for(i=0;i<10;i++)
    {
        if (i == 4)
            goto *codetable[opcode];
    }

    LABEL1:
    return;
}