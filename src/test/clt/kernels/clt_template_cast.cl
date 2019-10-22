
template< typename T >
struct X
{
    template< typename T2> struct Y
    {
        operator T() const;
    };
};

template< typename T >
template< typename T2 >
X<T>::Y<T2>::operator T() const
{
}

namespace Other
{

struct X
{
    operator u32();
    template< typename T >
    operator T*() const;
    template< typename T >
    operator T() const;
    template< typename T >
    operator T();
};

X::operator u32()
{
}

template<typename T>
X::operator T() const
{
}

template<typename T>
X::operator T()
{
}

template<typename T>
X::operator T*() const
{
}

}

void kmain(u32 index, u32 total)
{
}