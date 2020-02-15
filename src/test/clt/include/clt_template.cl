namespace UnitTest_Template
{

template <int X, int Y>
struct Test;

template <int X, int Y>
struct Test
{
    template <int Z>
    struct Test2;

    template <int Z>
    static Test2<Z> getZ();
    static Test2<X> getX();
    static Test2<Y> getY();
    static Test2<0> getZero();
};

template <int X, int Y>
template <int Z>
struct Test<X, Y>::Test2<Z>
{
    int m_values[Z];
};

template <int X, int Y>
template <int Z>
Test<X, Y>::Test2<Z> Test<X, Y>::getZ()
{
}

template <int X, int Y>
Test<X, Y>::Test2<X> Test<X, Y>::getX()
{
}

template <int X, int Y>
Test<X, Y>::Test2<Y> Test<X, Y>::getY()
{
}

template <int X, int Y>
Test<X, Y>::Test2<0> Test<X, Y>::getZero()
{
}

void kmain(u32 index, u32 total)
{
    Test<1, 1>::getX();
    Test<1, 4>::getY();
    Test<1, 3>::getZ<3>();
    Test<1, 4>::getZero();
}

}