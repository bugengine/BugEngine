#if 0
template< typename T >
struct X
{
};

template< >
struct X<i32>
{
    typedef u32 Y;
};

template< typename T >
struct Z
{
    X<T>::Y m_y;
};
#endif
void kmain(u32 index, u32 total)
{
    //X>i32> x;
}
