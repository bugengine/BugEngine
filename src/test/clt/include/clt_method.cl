namespace UnitTest_Method
{
    
namespace NS
{

struct B
{
    B();
    ~B();
    void doStuff()
    {
    }
};

B::B()
{
}

B::~B()
{
}

struct A
{
    A();
    A(int x);
    ~ A();

    inline void testInline()
    {
        this;
    }
    inline void testInline2();
    static void testStatic()
    {
    }
    void testVoid();
    void operator++();
    void operator++(int);
};

}

struct Z
{
    struct Y
    {
        struct X
        {
            operator const u32*() const;
            u32 operator ++();
            ~X();
        };
    };
};

Z::Y::X::operator const u32*() const
{
    return 0;
}

u32 Z::Y::X::operator ++()
{
}

Z::Y::X::~X()
{

}

template< typename T >
struct X
{
    template< typename T2> struct Y
    {
        operator T() const;
    };
    struct Z
    {
    };
};


template< typename T >
static typename X<T>::Z operation(const T& t);

void kmain(u32 index, u32 total)
{
}

}