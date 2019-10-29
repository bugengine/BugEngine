template< typename T >
struct vector
{
    u32 m_size;
    u32 m_capacity;
    T*  m_data;
    struct iterator{};
};

template< typename T >
struct DataStore
{
    typename vector<T>::iterator    m_storage;
    template< typename U >
    struct Storage
    {
        typename vector<U>::iterator    m_storage;
        T*                              m_dataType;

        template< typename X >
        typename vector<X>::iterator    cast();
    };
};

template< typename X >
struct IntermediateStore
{
    typename DataStore<X>::template Storage<u32> storage;
};

void kmain(u32 index, u32 total)
{
    IntermediateStore<i32> intermediateStorage;
}