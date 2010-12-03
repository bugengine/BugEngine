/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_CORE_MEMORY_NEW_INL_
#define BE_CORE_MEMORY_NEW_INL_
/*****************************************************************************/

void* operator new(size_t size) throw(std::bad_alloc)
{
    return BugEngine::gameArena().alloc(size, 16);
}

void* operator new(size_t size, size_t alignment) throw(std::bad_alloc)
{
    return BugEngine::gameArena().alloc(size, alignment);
}

void* operator new[](size_t size) throw(std::bad_alloc)
{
    return BugEngine::gameArena().alloc(size, 16);
}

void operator delete(void* ptr) throw()
{
    return BugEngine::gameArena().free(ptr);
}

void operator delete[](void* ptr) throw()
{
    return BugEngine::gameArena().free(ptr);
}

/*****************************************************************************/
#endif

