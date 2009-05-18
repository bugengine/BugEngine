#include    <stdio.h>
int do_test();

int main(int argc, char *argv[])
{
    if(argc > 1)
    {
        freopen(argv[1], "w", stdout);
    }
    return do_test();
}
