template< typename T >
struct Test
{
    struct X;
};

template< typename T >
struct Test<T>::X
{
    T m_t;
};


template< typename T >
struct Test<T*>
{
    struct X
    {
        T& m_testPtr;
    };
};

template< typename T >
struct Test<const T*>
{
    struct X
    {
        const T& m_testConstPtr;
    };
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
