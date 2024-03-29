/* BugEngine <bugengine.devel@gmail.com>
   see LICENSE for detail */

#ifndef BE_MINITL_POOL_
#define BE_MINITL_POOL_
/**************************************************************************************************/
#include <bugengine/minitl/stdafx.h>
#include <bugengine/kernel/interlocked.hh>
#include <bugengine/kernel/interlocked_stack.hh>
#include <bugengine/minitl/allocator.hh>

namespace minitl {

template < typename T >
class pool
{
private:
    struct node : public minitl::istack< node >::node
    {
    };
    enum
    {
        ElementSize = sizeof(T)
    };
    istack< node >        m_items;
    Allocator::Block< T > m_pool;
    T*                    m_end;

public:
    pool(Allocator& allocator, u64 capacity, u64 alignment = be_alignof(T));
    ~pool();

    T* allocate();
    template < typename T1 >
    T* allocate(const T1& t1);
    template < typename T1, typename T2 >
    T* allocate(const T1& t1, const T2& t2);
    template < typename T1, typename T2, typename T3 >
    T*   allocate(const T1& t1, const T2& t2, const T3& t3);
    void release(T* t);

    void swap(pool< T >& other);
};

}  // namespace minitl

#include <bugengine/minitl/inl/pool.inl>

/**************************************************************************************************/
#endif
