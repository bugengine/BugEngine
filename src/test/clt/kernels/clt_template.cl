template< int X, int Y>
struct Test
{
    template< int Z >
    struct Test2
    {
        static void doStuff();
    };
};

void kmain(u32 index, u32 total)
{
    Test<1, 1>::Test2<3>::doStuff();
    Test<2, 2>::Test2<3>::doStuff();
}

