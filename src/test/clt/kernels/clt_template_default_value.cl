
template< typename T >
struct Singleton
{
    T& getInstance()
    {
        static T s_instance;
        return s_instance;
    };
};

struct S : public Singleton<S>
{
    void main(u32 index, u32 total)
    {
    }
};

void kmain(u32 index, u32 total)
{
    S::getInstance().main(index, total);
}
