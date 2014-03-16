/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#ifndef BE_CORE_MEMORY_ALLOCATORS_GENERAL_HH_
#define BE_CORE_MEMORY_ALLOCATORS_GENERAL_HH_
/**************************************************************************************************/
#include    <core/stdafx.h>
#include    <minitl/allocator.hh>

namespace BugEngine
{

class be_api(CORE) GeneralAllocator : public minitl::Allocator
{
public:
    GeneralAllocator();
    ~GeneralAllocator();
protected:
    virtual void* internalAlloc(u64 size, u64 alignment) override;
    virtual bool  internalResize(void* ptr, u64 size) override;
    virtual void* internalRealloc(void* ptr, u64 size, u64 alignment) override;
    virtual void  internalFree(const void* pointer) override;
};

}

/**************************************************************************************************/
#endif

