template< u32 X >
struct F
{
    enum { Value = F<X-1>::Value + F<X-2>::Value };
};

template< >
struct F<1>
{
    enum { Value = 1 };
};

template< >
struct F<0>
{
    enum { Value = 1 };
};

void kmain(u32 current, u32 total)
{
    u32 x = F<72>::Value;
}