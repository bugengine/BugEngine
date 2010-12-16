/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#include    <rtti/stdafx.h>
#include    <rtti/engine/classinfo.script.hh>
#include    <rtti/engine/propertyinfo.script.hh>
#include    <rtti/engine/methodinfo.script.hh>
#include    <rtti/engine/wrapper.hh>

namespace BugEngine { namespace RTTI
{

ClassInfo::ClassInfo(const inamespace& name, ref<const ClassInfo> parent, ref<const ClassInfo> metaclass, u32 size)
    :   name(name)
    ,   parent(parent)
    ,   metaclass(metaclass)
    ,   size(size)
    ,   m_properties(rttiArena())
    ,   m_methods(rttiArena())
{
}

ClassInfo::~ClassInfo()
{
}

void ClassInfo::copy(const void* src, void* dst) const
{
}

void ClassInfo::destroy(void* src) const
{
}

}}
