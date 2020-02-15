namespace UnitTest_Namespace
{

namespace A
{
    struct X;
}

struct A::X
{
};

namespace
{

struct Y
{
};

}

void kmain(u32 index, u32 total)
{
    A::X x;
    Y y;
}

}
