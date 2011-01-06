/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#include   <rtti/stdafx.h>
#include   <rtti/namespace.script.hh>
#include   <rtti/engine/propertyinfo.script.hh>
#include   <rtti/value.hh>

namespace BugEngine { namespace RTTI
{

class Namespace::PropertyInfo : public RTTI::PropertyInfo
{
private:
    const Value m_value;
public:
    PropertyInfo(Value v);
    ~PropertyInfo();

    virtual Value get(Value& from) const override;
};

Namespace::PropertyInfo::PropertyInfo(Value v)
    :   RTTI::PropertyInfo(v.type(), RTTI::PropertyInfo::Get)
    ,   m_value(v)
{
}

Namespace::PropertyInfo::~PropertyInfo()
{
}

Value Namespace::PropertyInfo::get(Value& object) const
{
    return m_value;
}

Namespace::Namespace()
    :   m_classInfo(ref<RTTI::ClassInfo>::create(rttiArena(), be_typeid< Namespace >::klass()))
{
}

Namespace::~Namespace()
{
}

weak<Namespace> Namespace::getOrCreateNamespace(const inamespace& name)
{
    weak<Namespace> ns = this;
    for(size_t i = 0; i < name.size()-1; ++i)
    {
        weak<const RTTI::PropertyInfo> prop = ns->m_classInfo->getProperty(name[i]);
        if(! prop)
        {
            ref<Namespace> newns = ref<Namespace>::create(rttiArena());
            ref<const PropertyInfo> prop = ref<const PropertyInfo>::create(rttiArena(), Value(newns, TypeInfo(newns->m_classInfo, TypeInfo::RefPtr, TypeInfo::Mutable)));
            ns->m_classInfo->addProperty(name[i], prop);
            ns = newns;
        }
        else
        {
            be_assert(be_typeid< ref<Namespace> >::type() <= prop->type, "property of invalid type");
            Value v = prop->get(Value(ns));
            ns = v.as< ref<Namespace> >();
        }
    }
    return ns;
}

void Namespace::registerClassRoot(weak<const ClassInfo> klass)
{
    be_assert(be_typeid<void>::type() <= be_typeid<void>::type(), "internal error");
    be_assert(be_typeid<void>::type() <= be_typeid< Namespace >::type(), "internal error");
    be_assert(be_typeid< ref<minitl::refcountable> >::type() <= be_typeid< ref<Namespace> >::type(), "internal error");
    be_assert(be_typeid< ref< const Namespace> >::type() <= be_typeid< ref<Namespace> >::type(), "internal error");
    be_assert(!(be_typeid< ref< Namespace> >::type() <= be_typeid< ref<const Namespace> >::type()), "internal error");
    be_assert(!(be_typeid< ref< Namespace> >::type() <= be_typeid< ref<minitl::refcountable> >::type()), "internal error");
    weak<Namespace> ns = getOrCreateNamespace(klass->name);
    be_assert_recover(ns, "impossible to create namespace %s" | klass->name, return);
    ref<const PropertyInfo> prop = ref<const PropertyInfo>::create(rttiArena(), Value(klass));
    ns->m_classInfo->addProperty(klass->name[klass->name.size()-1], prop);
    for(minitl::intrusive_list<const ClassInfo>::iterator it = klass->m_children.begin(); it != klass->m_children.end(); ++it)
    {
        registerClassRoot(it.operator->());
    }
}

}}
