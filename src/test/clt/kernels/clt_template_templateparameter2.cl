template< class T >
class W
{
    T* t;
};

template< template<typename V> class T >
class S
{
    T<void>*   t;
public:
    int main() { return t->main(); }
};

template<typename X>
class Cls
{
    S<Cls>*  s;
    W<Cls>*  w;
public:
    int main() { return s->main(); }
};

template<>
class Cls<void>
{
public:
    int main() { return 0; }
};

void kmain(u32 index, u32 total)
{
    Cls<u32> c;
    c.main();
}
