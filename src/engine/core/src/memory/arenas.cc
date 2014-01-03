/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#include    <core/stdafx.h>
#include    <core/memory/allocators/general.hh>

namespace BugEngine { namespace Arena
{

minitl::Allocator& general()
{
    static GeneralAllocator s_allocator;
    return s_allocator;
}

minitl::Allocator& temporary()
{
    return general();
}

minitl::Allocator& stack()
{
    return general();
}

minitl::Allocator& debug()
{
    return general();
}

}}
