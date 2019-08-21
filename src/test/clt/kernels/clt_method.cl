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


void kmain(u32 index, u32 total)
{
}
