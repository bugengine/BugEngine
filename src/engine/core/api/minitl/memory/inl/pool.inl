/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_MINITL_MEMORY_POOL_INL_
#define BE_MINITL_MEMORY_POOL_INL_
/*****************************************************************************/
#include    <core/memory/malloc.hh>

namespace minitl
{

template< int ARENA, typename T >
pool<ARENA, T>::pool(size_t capacity, size_t alignment)
:   m_pool(capacity, alignment)
,   m_end(&m_pool[capacity])
{
    for(size_t i = 0; i < capacity-1; ++i)
        m_items.push((inode*)(&m_pool[i]));
}

template< int ARENA, typename T >
pool<ARENA, T>::~pool()
{
}

template< int ARENA, typename T >
T* pool<ARENA, T>::allocate()
{
    void* result = (void*)m_items.pop();
    be_assert(result >= m_pool && result < m_end, "allocated a node that is outside the node range");
    new(result) T;
    return (T*)result;
}


template< int ARENA, typename T >
template< typename T1 >
T* pool<ARENA, T>::allocate(const T1& t1)
{
    void* result = (void*)m_items.pop();
    be_assert(result >= m_pool && result < m_end, "allocated a node that is outside the node range");
    return new(result) T(t1);
}

template< int ARENA, typename T >
template< typename T1, typename T2 >
T* pool<ARENA, T>::allocate(const T1& t1, const T2& t2)
{
    void* result = (void*)m_items.pop();
    be_assert(result >= m_pool && result < m_end, "allocated a node that is outside the node range");
    return new(result) T(t1, t2);
}

template< int ARENA, typename T >
template< typename T1, typename T2, typename T3 >
T* pool<ARENA, T>::allocate(const T1& t1, const T2& t2, const T3& t3)
{
    void* result = (void*)m_items.pop();
    be_assert(result >= m_pool && result < m_end, "allocated a node that is outside the node range");
    return new(result) T(t1, t2, t3);
}

template< int ARENA, typename T >
void pool<ARENA, T>::release(T* t)
{
    t->~T();
    be_assert(t >= m_pool && t < m_end, "releasing a node that is outside the node range");
    m_items.push((inode*)t);
}

}

/*****************************************************************************/
#endif

