template< typename T >
struct list
{
    T* m_head;
    T* m_tail;
    typedef T* iterator;
    typedef const T* const_iterator;
};

template< typename T >
struct vector
{
    T* m_values;
    typedef T* iterator;
    typedef const T* const_iterator;
};


template< typename T, template< typename T > typename container >
struct abstract_container
{
    typedef abstract_container<T, container> _self;
    typedef abstract_container<T, vector> _vector;
    typedef abstract_container<T, list> _list;
    typedef typename container<T>::iterator iterator;
    typedef typename container<T>::const_iterator const_iterator;
    container<T> m_container;
};

void kmain(u32 index, u32 total)
{
    abstract_container<u32, vector> vec;
    abstract_container<u32, list> lst;
}