struct Test
{
    i32 m_testValue;
};

struct Test2 : private Test
{
};

class Test3 : Test2
{
};

int kmain(u32 index, u32 total)
{
    Test3 test;
}
