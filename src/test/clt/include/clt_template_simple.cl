namespace UnitTest_TemplateSimple
{

template< typename X >
struct Test;

template< typename T >
struct Test
{
    struct Test2;
};

template< typename T2 >
struct Test<T2>::Test2
{
    Test2();
    T2 test;
};

void kmain(u32 index, u32 total)
{
    Test<u32>::Test2 x = 0;
    Test<i32>::Test2 y = 0;
}

}