template< typename T >
class Singleton
{
public:
    static T& getInstance()
    {
        static T s_instance;
        return s_instance;
    }
};


class A : public Singleton<A>
{
public:
    void main(u32 index, u32 total);
};


void A::main(u32 index, u32 total)
{
}


void kmain(u32 index, u32 total)
{
    A::getInstance().main(index, total);
}
