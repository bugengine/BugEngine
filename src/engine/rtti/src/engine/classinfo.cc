/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#include    <rtti/stdafx.h>
#include    <rtti/classinfo.script.hh>
#include    <rtti/engine/propertyinfo.script.hh>
#include    <rtti/engine/methodinfo.script.hh>
#include    <rtti/value.inl>


namespace BugEngine { namespace RTTI
{

ClassInfo::ClassInfo(const inamespace& name, ref<const ClassInfo> parent, u32 size, i32 offset)
    :   Namespace()
    ,   name(name)
    ,   parent(parent)
    ,   size(size)
    ,   offset(offset)
    ,   properties(rttiArena())
    ,   methods(rttiArena())
    ,   tags(rttiArena())
    ,   copyconstructor(0)
    ,   destructor(0)
{
    if (parent)
    {
        properties.insert(parent->properties.begin(), parent->properties.end());
        methods.insert(parent->methods.begin(), parent->methods.end());
    }
}

ClassInfo::ClassInfo(const inamespace& name, ref<const ClassInfo> parent)
    :   Namespace()
    ,   name(name)
    ,   parent(parent)
    ,   size(0)
    ,   offset(0)
    ,   properties(rttiArena())
    ,   methods(rttiArena())
    ,   tags(rttiArena())
    ,   copyconstructor(0)
    ,   destructor(0)
{
    if (parent)
    {
        properties.insert(parent->properties.begin(), parent->properties.end());
        methods.insert(parent->methods.begin(), parent->methods.end());
    }
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

void ClassInfo::addProperty(istring name, const PropertyInfo& prop)
{
    be_assert(properties.find(name) == properties.end(), "Property %s already exists in class %s!" | name | this->name);
    properties.insert(std::make_pair(name, prop));
}

void ClassInfo::removeProperty(istring name)
{
    properties.erase(name);
}

void ClassInfo::addMethod(istring name, const MethodInfo& m)
{
    be_assert(methods.find(name) == methods.end(), "Method %s already exists in class %s!" | name | this->name);
    methods.insert(std::make_pair(name, m));
}

void ClassInfo::removeMethod(istring name)
{
    methods.erase(name);
}

Value ClassInfo::get(Value& from, istring name) const
{
    minitl::hashmap< istring, const PropertyInfo >::const_iterator it = properties.find(name);
    if (it != properties.end())
        return it->second.get(from);
    else
    {
        minitl::hashmap< istring, const MethodInfo >::const_iterator it = methods.find(name);
        if (it != methods.end())
            return Value(Value::ByRef(it->second));
        else
        {
            return Value();
        }
    }
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

Value ClassInfo::operator()(Value* params, size_t nparams) const
{
    return constructor(params, nparams);
}

Value ClassInfo::getTag(const TypeInfo& type) const
{
    if(!tags.empty())
        return tags[0];
    else
        return Value();
}

}}
