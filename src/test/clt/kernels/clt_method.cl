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
};

}

/*void A::testInline2()
{
}

void A::testStatic()
{
}*/


void kmain(u32 index, u32 total)
{
}
