template< typename T1, typename T2 >
struct Test
{
    T1 m_test1;
    T2 m_test2;
};

template< typename T >
struct Test<T, T>
{
    T m_test[2];
};

template< typename T1, typename T2 >
struct Test<const T1, const T2>
{
    T1 m_test1;
    T2 m_test2;
};


void kmain(u32 index, u32 total)
{
    Test<const u32, const u32> z;
}