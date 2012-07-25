/* BugEngine / 2008-2012  Nicolas MERCIER <mercier.nicolas@gmail.com>
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
