/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_CORE_MEMORY_ALLOCATORS_STACK_HH_
#define BE_CORE_MEMORY_ALLOCATORS_STACKL_HH_
/*****************************************************************************/
#include    <core/memory/malloc.hh>

namespace BugEngine
{

class be_api(CORE) StackAllocator : public Allocator
{
public:
    StackAllocator();
    ~StackAllocator();
protected:
    virtual void* internalAlloc(size_t size, size_t alignment) override;
    virtual bool  internalResize(void* ptr, size_t size) override;
    virtual void* internalRealloc(void* ptr, size_t size, size_t alignment) override;
    virtual void  internalFree(const void* pointer) override;
};

}

/*****************************************************************************/
#endif

