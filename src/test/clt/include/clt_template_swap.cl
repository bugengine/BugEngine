namespace UnitTest_TemplateSwap
{

template< typename X >
struct A
{
    template< typename Y >
    struct B
    {
        operator typename A<Y>::template B<X>();
        typename A<Y>::template B<X> swap();
    };
};

template< typename X >
template< typename Y >
typename A<Y>::template B<X> A<X>::B<Y>::swap()
{
    return A<Y>::template B<X>();
}

template< typename X >
template< typename Y >
A<X>::B<Y>::operator typename A<Y>::template B<X>()
{
    return A<Y>::template B<X>();
}

void kmain(u32 index, u32 total)
{
    A<u32>::B<i32> test;
}

}
