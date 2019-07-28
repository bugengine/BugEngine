template< typename T, template< typename T > typename container >
struct abstract_container
{
    container<T> m_container;
};

template< typename T >
struct list
{
    T* m_head;
    T* m_tail;
};

template< typename T >
struct vector
{
    T m_values[10];
};

void kmain(u32 index, u32 total)
{
    abstract_container<u32, vector> vec;
    abstract_container<u32, list> lst;
}