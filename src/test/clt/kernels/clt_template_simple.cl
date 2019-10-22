template< typename X >
struct Test;

template< typename T >
struct Test
{
    ~Test();
    Test();
    Test(const Test& other);
};

template< typename T >
Test<T>::Test(const Test& other)
{
}

void kmain(u32 index, u32 total)
{
    Test<u32> x;
    Test<i32> y;
}
