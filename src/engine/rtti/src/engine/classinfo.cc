/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#include    <rtti/stdafx.h>
#include    <rtti/engine/classinfo.script.hh>
#include    <rtti/engine/propertyinfo.script.hh>
#include    <rtti/engine/methodinfo.script.hh>
#include    <rtti/engine/wrapper.hh>

namespace BugEngine { namespace RTTI
{

ClassInfo::ClassInfo(const inamespace& name, ref<const ClassInfo> parent, ref<ClassInfo> metaclass, u32 size, i32 offset)
    :   Namespace(metaclass)
    ,   name(name)
    ,   parent(parent)
    ,   size(size)
    ,   offset(offset)
    ,   m_properties(rttiArena())
    ,   m_inTree(true)
{
    if(parent)
    {
        parent->m_children.push_back(*this);
    }
    else
    {
        be_assert(name == inamespace("void"), "only void (root) can have no parent");
    }
}

ClassInfo::ClassInfo(ref<const ClassInfo> parent, ref<ClassInfo> metaclass)
    :   Namespace(metaclass)
    ,   name("anonymous")
    ,   parent(parent)
    ,   size(0)
    ,   offset(0)
    ,   m_properties(rttiArena())
    ,   m_inTree(false)
{
    be_assert(parent, "class \"%s\" has no parent; only void (root) can have no parent" | name);
}

ClassInfo::~ClassInfo()
{
    be_assert(m_children.empty(), "destroying class \"%s\" that still has children" | name);
    if(parent && m_inTree)
        unhook();
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
