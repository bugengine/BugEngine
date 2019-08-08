namespace F
{
    template< typename T >
    struct X
    {
        T x;
    };

    template< typename T >
    struct EnumWrapper
    {
        enum Enum
        {
            Value0,
            Value1 = 1,
            Value2 = Value1 + 1,
            Value5 = 5,
            Value5bis = 5,
            Value6,
            ValueEnd = T::Max
        };
    };
}

struct ByteEnum
{
    enum Enum { Max = 255 };
};

void kmain(u32 index, u32 total)
{
    F::X<F::EnumWrapper<ByteEnum>::Enum> e;
}