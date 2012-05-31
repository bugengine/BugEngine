/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#include    <core/stdafx.h>
#include    <core/memory/malloc.hh>
#include    <core/memory/allocators/general.hh>

namespace BugEngine { namespace Arena
{

Allocator& general()
{
    static GeneralAllocator s_allocator;
    return s_allocator;
}

Allocator& temporary()
{
    return general();
}

Allocator& stack()
{
    return general();
}

Allocator& debug()
{
    return general();
}

}}
