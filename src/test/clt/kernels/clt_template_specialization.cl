template< typename T >
struct Test
{
    T m_test;
};

template< typename T >
struct Test<T*>
{
    T& m_testPtr;
};

template< >
struct Test<i32>
{
    i32  m_testArray[10];
};

void kmain(u32 index, u32 total)
{
    Test<u32> x;
    Test<i32> y1;
    Test<i32> y2;
    Test<i32*> z;
    Test<u32*> w;
}