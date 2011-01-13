/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#include    <rtti/stdafx.h>
#include    <rtti/classinfo.script.hh>
#include    <rtti/engine/propertyinfo.script.hh>
#include    <rtti/engine/methodinfo.script.hh>

namespace BugEngine { namespace RTTI
{

ClassInfo::ClassInfo(const inamespace& name, ref<const ClassInfo> parent, ref<ClassInfo> metaclass, u32 size, i32 offset)
    :   Namespace(metaclass)
    ,   name(name)
    ,   parent(parent)
    ,   size(size)
    ,   offset(offset)
    ,   m_properties(rttiArena())
{
}

ClassInfo::ClassInfo(const inamespace& name, ref<const ClassInfo> parent)
    :   Namespace(ref<ClassInfo>())
    ,   name(name)
    ,   parent(parent)
    ,   size(0)
    ,   offset(0)
    ,   m_properties(rttiArena())
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

void ClassInfo::addProperty(const istring& name, ref<const PropertyInfo> prop)
{
    m_properties[name] = prop;
}

void ClassInfo::removeProperty(const istring& name)
{
    m_properties.erase(name);
}

weak<const PropertyInfo> ClassInfo::getProperty(const istring& name) const
{
    minitl::hashmap< istring, ref<const PropertyInfo> >::const_iterator it = m_properties.find(name);
    if (it != m_properties.end())
        return it->second;
    else
        return weak<const PropertyInfo>();
}

bool ClassInfo::isA(weak<const ClassInfo> klass) const
{
    weak<const ClassInfo> ci = this;
    while(ci)
    {
        if(ci == klass)
            return true;
        ci = ci->parent;
    }
    return false;
}

}}
