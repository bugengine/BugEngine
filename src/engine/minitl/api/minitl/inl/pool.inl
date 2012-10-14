/* BugEngine / 2008-2012  Nicolas MERCIER <mercier.nicolas@gmail.com>
   see LICENSE for detail */

#ifndef BE_MINITL_MEMORY_POOL_INL_
#define BE_MINITL_MEMORY_POOL_INL_
/*****************************************************************************/


namespace minitl
{

template< typename T >
pool<T>::pool(Allocator& allocator, size_t capacity, size_t alignment)
:   m_pool(allocator, capacity, alignment)
,   m_end(&m_pool[capacity])
{
    for (size_t i = 0; i < capacity; ++i)
        m_items.push((node*)(&m_pool[i]));
}

template< typename T >
pool<T>::~pool()
{
}

template< typename T >
T* pool<T>::allocate()
{
    void* result = (void*)m_items.pop();
    be_assert(result >= m_pool && result < m_end, "allocated a node that is outside the node range");
    new(result) T;
    return (T*)result;
}


template< typename T >
template< typename T1 >
T* pool<T>::allocate(const T1& t1)
{
    void* result = (void*)m_items.pop();
    be_assert(result >= m_pool && result < m_end, "allocated a node that is outside the node range");
    return new(result) T(t1);
}

template< typename T >
template< typename T1, typename T2 >
T* pool<T>::allocate(const T1& t1, const T2& t2)
{
    void* result = (void*)m_items.pop();
    be_assert(result >= m_pool && result < m_end, "allocated a node that is outside the node range");
    return new(result) T(t1, t2);
}

template< typename T >
template< typename T1, typename T2, typename T3 >
T* pool<T>::allocate(const T1& t1, const T2& t2, const T3& t3)
{
    void* result = (void*)m_items.pop();
    be_assert(result >= m_pool && result < m_end, "allocated a node that is outside the node range");
    return new(result) T(t1, t2, t3);
}

template< typename T >
void pool<T>::release(T* t)
{
    t->~T();
    be_assert(t >= m_pool && t < m_end, "releasing a node that is outside the node range");
    m_items.push((node*)t);
}

template< typename T >
void pool<T>::swap(pool<T>& other)
{
    minitl::swap(m_items, other.m_items);
    m_pool.swap(other.m_pool);
    minitl::swap(m_end, other.m_end);
}

}

/*****************************************************************************/
#endif

