/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#include    <rtti/stdafx.h>
#include    <rtti/namespace.script.hh>
#include    <rtti/classinfo.script.hh>
#include    <rtti/engine/propertyinfo.script.hh>
#include    <rtti/value.hh>

namespace BugEngine { namespace RTTI
{

class Namespace::MetaPropertyInfo : public RTTI::PropertyInfo
{
private:
    const Value m_value;
public:
    MetaPropertyInfo(Value v);
    ~MetaPropertyInfo();

    static Value get(weak<const PropertyInfo> _this, void* from);
};

Namespace::MetaPropertyInfo::MetaPropertyInfo(Value v)
    :   RTTI::PropertyInfo(v.type(), &get, 0)
    ,   m_value(v)
{
}

Namespace::MetaPropertyInfo::~MetaPropertyInfo()
{
}

Value Namespace::MetaPropertyInfo::get(weak<const PropertyInfo> _this, void* from)
{
    return be_checked_cast< const MetaPropertyInfo >(_this)->m_value;
}

//-------------------------------------------------------------------------

Namespace::Namespace(ref<ClassInfo> metaclass)
    :   metaclass(metaclass)
    ,   m_propertyCount(0)
{
}

Namespace::Namespace()
    :   metaclass(ref<ClassInfo>::create(rttiArena(), inamespace("namespace"), be_typeid<Namespace>::klass()))
    ,   m_propertyCount(0)
{
}

Namespace::~Namespace()
{
}

weak<const Namespace> Namespace::getNamespace(const inamespace& name, CreationPolicy policy) const
{
    Value ns = Value(weak<const Namespace>(this), metaclass);
    for(size_t i = 0; i < name.size()-1; ++i)
    {
        Value v = ns[name[i]];
        weak<const Namespace> namespace_;
        if ((ns.type().type & TypeInfo::TypeMask) == TypeInfo::ConstWeakPtr)
            namespace_ = ns.as< weak<const Namespace> >();
        else
            namespace_ = ns.as< ref<const Namespace> >();
        if(!v)
        {
            if(policy == Create)
            {
                ref<const Namespace> newns = ref<const Namespace>::create(rttiArena());
                namespace_->add(name[i], Value(newns, TypeInfo(newns->metaclass, TypeInfo::RefPtr, TypeInfo::Mutable)));
                namespace_->m_propertyCount++;
                ns = Value(weak<const Namespace>(newns), newns->metaclass);
            }
            else
            {
                return weak<const Namespace>();
            }
        }
        else
        {
            ns = v;
            if(! (be_typeid< ref<const Namespace> >::type() <= ns.type() || be_typeid< weak<const Namespace> >::type() <= ns.type()))
            {
                inamespace shortname(name);
                for(size_t j = 1; j < i; ++j)
                    shortname.push_back(name[i]);
                be_error("Unable to convert value %s of type %s to a Namespace" | name | ns.type().name());
                return weak<const Namespace>();
            }
        }
    }

    if ((ns.type().type & TypeInfo::TypeMask) == TypeInfo::ConstWeakPtr)
        return ns.as< weak<const Namespace> >();
    else
        return ns.as< ref<const Namespace> >();
}

void Namespace::add(const inamespace& name, const Value& value) const
{
    weak<const Namespace> ns = getNamespace(name, Create);
    if(ns)
    {
        ns->m_propertyCount++;
        ns->metaclass->addProperty(name[name.size()-1], ref<const MetaPropertyInfo>::create(rttiArena(), value));
    }
}

void Namespace::add(const istring& name, const Value& value) const
{
    m_propertyCount++;
    metaclass->addProperty(name, ref<const MetaPropertyInfo>::create(rttiArena(), value));
}

void Namespace::remove(const inamespace& name) const
{
    weak<const Namespace> ns = getNamespace(name, DoNotCreate);
    if(ns)
    {
        ns->m_propertyCount--;
        ns->metaclass->removeProperty(name[name.size()-1]);
        if(ns->m_propertyCount == 0 && name.size() > 1)
        {
            // remove weak reference before deleting the namespace
            ns = weak<const Namespace>();
            inamespace nsname = name;
            nsname.pop_back();
            remove(nsname);
        }
    }
}

void Namespace::remove(const istring& name) const
{
    m_propertyCount--;
    metaclass->removeProperty(name);
}

weak<const Namespace> Namespace::rttiRoot()
{
    static ref<Namespace> s_root = ref<Namespace>::create(rttiArena());
    return s_root;
}

}}
