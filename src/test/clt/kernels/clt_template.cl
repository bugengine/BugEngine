template< int X, int Y >
struct Test;

template< int X, int Y >
struct Test
{
    template< int Z >
    struct Test2
    {
        static void doStuff();
        template< int W >
        static void hop();
    };
};

template< int X, int Y >
template< int Z >
void Test<X, Y>::Test2< Z >::doStuff()
{
}

template< int X, int Y >
template< int Z >
template< int W >
void Test<X, Y>::Test2< Z >::hop()
{
}

void kmain(u32 index, u32 total)
{
    Test<1, 1>::Test2<3>::doStuff();
    Test<2, 2>::Test2<3>::doStuff();
}

