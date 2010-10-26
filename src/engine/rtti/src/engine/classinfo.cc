/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#include    <rtti/stdafx.h>
#include    <rtti/engine/classinfo.script.hh>
#include    <rtti/engine/wrapper.hh>

namespace BugEngine { namespace RTTI
{

void ClassInfo::copy(const void *src, void* dst) const
{
    memcpy(dst, src, size);
}

void ClassInfo::destroy(void*) const
{
}

ClassInfoRegistration::ClassInfoRegistration(const ClassInfo* classinfo)
{
    const ClassInfo* parent = classinfo->parent.m_ptr;
    if(parent != classinfo)
    {
        classinfo->next.m_ptr = parent->child.m_ptr;
        parent->child.m_ptr = classinfo;
    }
}

ClassInfoRegistration::~ClassInfoRegistration()
{
}

}}
