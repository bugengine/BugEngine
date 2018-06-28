namespace NS
{

struct B
{
};

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
    void operator++();
};


A::A()
{
}

A::A(int)
{
}

A::~A()
{
}

void A::testInline2()
{
}

void A::operator++()
{
}

}
void kmain(u32 index, u32 total)
{
}
