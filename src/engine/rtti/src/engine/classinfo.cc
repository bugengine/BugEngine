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
    ,   properties(rttiArena())
    ,   tags(rttiArena())
    ,   copyconstructor(0)
    ,   destructor(0)
{
}

ClassInfo::ClassInfo(const inamespace& name, ref<const ClassInfo> parent)
    :   Namespace(ref<ClassInfo>())
    ,   name(name)
    ,   parent(parent)
    ,   size(0)
    ,   offset(0)
    ,   properties(rttiArena())
    ,   tags(rttiArena())
    ,   copyconstructor(0)
    ,   destructor(0)
{
}

ClassInfo::~ClassInfo()
{
}

void ClassInfo::copy(const void* src, void* dst) const
{
    be_assert_recover(copyconstructor, "no copy for type %s" | name, return);
    (*copyconstructor)(src, dst);
}

void ClassInfo::destroy(void* src) const
{
    be_assert_recover(destructor, "no destructor for type %s" | name, return);
    (*destructor)(src);
}

void ClassInfo::addProperty(const istring& name, ref<const PropertyInfo> prop)
{
    be_assert(properties.find(name) == properties.end(), "Property %s already exists in class %s!" | name | this->name);
    properties[name] = prop;
}

void ClassInfo::replaceProperty(const istring& name, ref<const PropertyInfo> prop)
{
    properties[name] = prop;
}

void ClassInfo::removeProperty(const istring& name)
{
    properties.erase(name);
}

weak<const PropertyInfo> ClassInfo::getProperty(const istring& name) const
{
    minitl::hashmap< istring, ref<const PropertyInfo> >::const_iterator it = properties.find(name);
    if (it != properties.end())
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

Value ClassInfo::call(Value* params, size_t nparams) const
{
    be_assert_recover(callOperator != 0, "Object of type %s is not callable" | name, return Value());
    return callOperator->operator()(params, nparams);
}

Value ClassInfo::getTag(const TypeInfo& type) const
{
    return Value();
}

Value ClassInfo::getTag() const
{
    if(!tags.empty())
        return tags[0];
    else
        return Value();
}

}}
