
int main()
{
    int a = 8;

    while (a > 0)
    {
        switch(a)
        {
            case 1: a = 0;
                    break;
            case 2: a = 1;
                    break;
            default: a = 2;
                    break;
        };
    }

    return 0;
}

