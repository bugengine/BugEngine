/* BugEngine <bugengine.devel@gmail.com>
   see LICENSE for detail */

#ifndef BE_CORE_MEMORY_ALLOCATORS_POOL_HH_
#define BE_CORE_MEMORY_ALLOCATORS_POOL_HH_
/**************************************************************************************************/
#include <bugengine/core/stdafx.h>
#include <bugengine/minitl/allocator.hh>

namespace BugEngine {

class be_api(CORE) PoolAllocator : public minitl::Allocator
{
public:
    PoolAllocator();
    ~PoolAllocator();

protected:
    virtual void* internalAlloc(u64 size, u64 alignment) override;
    virtual bool  internalResize(void* ptr, u64 size) override;
    virtual void* internalRealloc(void* ptr, u64 size, u64 alignment) override;
    virtual void  internalFree(const void* pointer) override;
};

}  // namespace BugEngine

/**************************************************************************************************/
#endif
