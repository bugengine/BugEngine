/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#include    <core/stdafx.h>
#include    <core/memory/malloc.hh>
#include    <core/memory/allocators/general.hh>

namespace BugEngine
{

Allocator& gameArena()
{
    static GeneralAllocator s_allocator;
    return s_allocator;
}

Allocator& tempArena()
{
    return gameArena();
}

Allocator& debugArena()
{
    return gameArena();
}

Allocator& rttiArena()
{
    return gameArena();
}

Allocator& taskArena()
{
    return gameArena();
}

Allocator& inputArena()
{
    return gameArena();
}

}
