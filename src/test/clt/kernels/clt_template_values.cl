template< u32 X >
struct Fibonacci
{
    enum { Value = Fibonacci<X-1>::Value + Fibonacci<X-2>::Value };
};

template< >
struct Fibonacci<1>
{
    enum { Value = 1 };
};

template< >
struct Fibonacci<0>
{
    enum { Value = 1 };
};

void kmain(u32 current, u32 total)
{
    u32 x = Fibonacci<50>::Value;
}