template< typename T >
struct X
{
    template< typename U >
    struct Y;
    typename T::reference main();
};

template< typename T >
template< typename U >
struct X<T>::Y
{
    bool operator==(const Y& other) const;
};


template< typename T >
template< typename U >
bool X<T>::Y<U>::operator==(const Y<U>& other) const
{
    return false;
}

template< typename T >
typename T::reference X<T>::main()
{

}

void kmain(u32 index, u32 total)
{
    method();
}