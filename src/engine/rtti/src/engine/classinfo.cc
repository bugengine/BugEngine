/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#include    <rtti/stdafx.h>
#include    <rtti/classinfo.script.hh>
#include    <rtti/engine/propertyinfo.script.hh>
#include    <rtti/engine/methodinfo.script.hh>
#include    <rtti/value.inl>


namespace BugEngine { namespace RTTI
{

ClassInfo::ClassInfo(const inamespace& name_, ref<const ClassInfo> parent_, u32 size_, i32 offset_)
    :   Namespace()
    ,   name(name_)
    ,   parent(parent_)
    ,   size(size_)
    ,   offset(offset_)
    ,   properties(rttiArena())
    ,   methods(rttiArena())
    ,   m_tags(rttiArena())
    ,   copyconstructor(0)
    ,   destructor(0)
{
    if (parent)
    {
        properties.insert(parent->properties.begin(), parent->properties.end());
        methods.insert(parent->methods.begin(), parent->methods.end());
    }
}

ClassInfo::ClassInfo(const inamespace& name_, ref<const ClassInfo> parent_)
    :   Namespace()
    ,   name(name_)
    ,   parent(parent_)
    ,   size(0)
    ,   offset(0)
    ,   properties(rttiArena())
    ,   methods(rttiArena())
    ,   m_tags(rttiArena())
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

void ClassInfo::addProperty(istring propname, const PropertyInfo& prop)
{
    be_assert(properties.find(propname) == properties.end(), "Property %s already exists in class %s!" | propname | name);
    properties.insert(std::make_pair<istring, const PropertyInfo>(propname, prop));
}

void ClassInfo::removeProperty(istring propname)
{
    properties.erase(propname);
}

void ClassInfo::addMethod(istring methodname, const MethodInfo& m)
{
    be_assert(methods.find(methodname) == methods.end(), "Method %s already exists in class %s!" | methodname | name);
    methods.insert(std::make_pair<istring, const MethodInfo>(methodname, m));
}

void ClassInfo::removeMethod(istring methodname)
{
    methods.erase(methodname);
}

Value ClassInfo::get(Value& from, istring propname) const
{
    minitl::hashmap< istring, const PropertyInfo >::const_iterator it = properties.find(propname);
    if (it != properties.end())
        return it->second.get(from);
    else
    {
        minitl::hashmap< istring, const MethodInfo >::const_iterator it = methods.find(propname);
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
    while (ci)
    {
        if (ci == klass)
            return true;
        ci = ci->parent;
    }
    return false;
}

Value ClassInfo::operator()(Value* params, u32 nparams) const
{
    return constructor(params, nparams);
}

Value ClassInfo::getTag(const TypeInfo& type) const
{
    for (minitl::vector< Value >::iterator it = m_tags.begin(); it != m_tags.end(); ++it)
    {
        if (type <= it->type())
        {
            for (minitl::vector< Value >::const_iterator it2 = it+1; it2 != m_tags.end(); ++it2)
            {
                if (type <= it2->type())
                {
                    be_warning("several tags match for type %s: value selected is %s, conflict value %s" | type.name() | it->type().name() | it2->type().name());
                }
            }
            return Value(Value::ByRef(*it));
        }
    }
    return Value();
}

Value ClassInfo::getTag(ref<const ClassInfo> type) const
{
    return getTag(TypeInfo(type, TypeInfo::Class, TypeInfo::Mutable));
}

u32 ClassInfo::distance(weak<const ClassInfo> other) const
{
    weak<const ClassInfo> ci = this;
    u32 result = 0;
    while (ci)
    {
        if (ci == other)
            return result;
        ci = ci->parent;
        result++;
    }
    return TypeInfo::MaxTypeDistance;
}

void ClassInfo::clearTags()
{
    m_tags.clear();
}

void ClassInfo::clearProperties()
{
    properties.clear();
}

void ClassInfo::clearMethods()
{
    methods.clear();
}

}}
