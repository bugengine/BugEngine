/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_MINITL_MEMORY_POOL_INL_
#define BE_MINITL_MEMORY_POOL_INL_
/*****************************************************************************/
#include    <core/memory/malloc.hh>

namespace minitl
{

template< typename T >
pool<T>::pool(size_t capacity, size_t alignment)
:   m_pool(BugEngine::Malloc::internalAllocArray<T>(capacity, alignment))
,   m_end((char*)m_pool + be_align(sizeof(T),alignment)*capacity)
{
    const size_t aligned_size = be_align(sizeof(T),alignment);
    for(size_t i = 0; i < capacity-1; ++i)
        m_items.push((inode*)((char*)m_pool+i*aligned_size));
}

template< typename T >
pool<T>::~pool()
{
    BugEngine::Malloc::internalFree(m_pool);
}

template< typename T >
T* pool<T>::allocate()
{
    T* result = (T*)m_items.pop();
    be_assert(result >= m_pool && result < m_end, "allocated a node that is outside the node range");
    return result;
}

template< typename T >
void pool<T>::release(T* t)
{
    be_assert(t >= m_pool && t < m_end, "releasing a node that is outside the node range");
    m_items.push((inode*)t);
}

}

/*****************************************************************************/
#endif

