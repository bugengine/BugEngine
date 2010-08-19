/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_CORE_MEMORY_NEW_INL_
#define BE_CORE_MEMORY_NEW_INL_
/*****************************************************************************/

void* operator new(size_t size) throw(std::bad_alloc)
{
    return BugEngine::Memory<BugEngine::Arena::General>::alloc(size, 16);
}

void* operator new(size_t size, size_t alignment) throw(std::bad_alloc)
{
    return BugEngine::Memory<BugEngine::Arena::General>::alloc(size, alignment);
}

void* operator new[](size_t size) throw(std::bad_alloc)
{
    return BugEngine::Memory<BugEngine::Arena::General>::alloc(size, 16);
}

void operator delete(void* ptr) throw()
{
    return BugEngine::Memory<BugEngine::Arena::General>::free(ptr);
}

void operator delete[](void* ptr) throw()
{
    return BugEngine::Memory<BugEngine::Arena::General>::free(ptr);
}

/*****************************************************************************/
#endif

