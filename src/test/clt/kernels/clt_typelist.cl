template< typename T, typename Tail >
struct TypeList : public Tail
{
    enum { Size = 1 + Tail::Size };
};

template< typename T >
struct TypeList<T, void>
{
    enum { Size = 1 };
};

void kmain(u32 current, u32 total)
{
    typedef TypeList< u32, TypeList< i32, TypeList< u16, TypeList< i16, TypeList<u8, void> > > > > List;
    total = List::Size;
}

