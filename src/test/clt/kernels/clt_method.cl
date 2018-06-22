namespace NS
{

struct A
{
    A();
    A(int x);
    ~A();

    inline void testInline()
    {
        this;
    }
    inline void testInline2();
    static void testStatic()
    {
    }
    void operator++() {}
};


A::A()
{
}
//void A::operator++()
//{
//}

}

NS::A::A()
{
}

void NS::A::testInline2()
{
}

void NS::A::testStatic()
{
}


void kmain(u32 index, u32 total)
{
}
