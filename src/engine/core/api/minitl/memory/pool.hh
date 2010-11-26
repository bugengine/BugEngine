/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_MINITL_MEMORY_POOL_
#define BE_MINITL_MEMORY_POOL_
/*****************************************************************************/
#include    <minitl/interlocked/interlocked.hh>
#include    <core/threads/criticalsection.hh>
#include    <minitl/interlocked/stack.hh>

namespace minitl
{

template< int ARENA, typename T >
class pool
{
private:
    enum
    {
        ElementSize = sizeof(T)
    };
    istack<inode>                                           m_items;
    typename BugEngine::Memory<ARENA>::template Block<T>    m_pool;
    T*                                                      m_end;
public:
    pool(size_t capacity, size_t alignment = be_alignof(T));
    ~pool();
    
    T* allocate();
    template< typename T1 >
    T* allocate(const T1& t1);
    template< typename T1, typename T2 >
    T* allocate(const T1& t1, const T2& t2);
    template< typename T1, typename T2, typename T3 >
    T* allocate(const T1& t1, const T2& t2, const T3& t3);
    void release(T* t);
};

}

#include    "inl/pool.inl"

/*****************************************************************************/
#endif
