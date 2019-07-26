template< typename T >
struct Test
{
    template< typename T2 >
    struct NestedTest;
};


template< typename T >
template< typename T2 >
struct Test<T>::NestedTest
{
    template< typename T3 >
    struct NestedNestedTest
    {
        T3* t;
    };
    T   m_test;
    T2* m_testPtr;
};


template< typename U >
struct Test<U*>
{
    template< typename T2 >
    struct NestedTest
    {
        U m_x[10];
        T2 m_y[12];
    };
};


template< typename V >
struct Test<const V>
{
    template< typename T2 >
    struct NestedTestConst
    {
        V m_x[10];
        T2 m_y[12];
        T2* m_z;
    };
};


void kmain(u32 index, u32 total)
{
    Test<u32>::NestedTest<i32> x;
    Test<i32>::NestedTest<i32> y;
    Test<i32*>::NestedTest<i32> z;
    Test<i32* const>::NestedTestConst<i32> z2;
}