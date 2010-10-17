/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#include    <rtti/stdafx.h>
#include    <rtti/engine/classinfo.script.hh>

namespace BugEngine { namespace RTTI
{

void ClassInfo::copy(const void *src, void* dst) const
{
    memcpy(dst, src, size);
}

void ClassInfo::destroy(void*) const
{
}

}}
