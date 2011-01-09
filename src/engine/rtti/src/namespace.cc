/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#include    <rtti/stdafx.h>
#include    <rtti/namespace.script.hh>
#include    <rtti/engine/classinfo.script.hh>
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

static ref<ClassInfo> metametaclass()
{
    static ref<ClassInfo> s_metametaClass = ref<ClassInfo>::create(rttiArena(), be_typeid<ClassInfo>::klass(), ref<ClassInfo>());
    return s_metametaClass;
}

Namespace::Namespace(ref<ClassInfo> metaclass)
    :   metaclass(metaclass)
{
}

Namespace::Namespace()
    :   metaclass(ref<RTTI::ClassInfo>::create(rttiArena(), be_typeid<Namespace>::klass(), metametaclass()))
{
}

Namespace::~Namespace()
{
}

weak<const Namespace> Namespace::getOrCreateNamespace(const inamespace& name)
{
    Value ns = Value(weak<const Namespace>(this), metaclass);
    for(size_t i = 0; i < name.size(); ++i)
    {
        Value v = ns[name[i]];
        weak<const Namespace> namespace_;
        if ((ns.type().type & TypeInfo::TypeMask) == TypeInfo::ConstWeakPtr)
            namespace_ = ns.as< weak<const Namespace> >();
        else
            namespace_ = ns.as< ref<const Namespace> >();
        if(!v)
        {
            ref<const Namespace> newns = ref<const Namespace>::create(rttiArena());
            ref<const MetaPropertyInfo> prop = ref<const MetaPropertyInfo>::create(rttiArena(), Value(newns, TypeInfo(newns->metaclass, TypeInfo::RefPtr, TypeInfo::Mutable)));
            namespace_->metaclass->addProperty(name[i], prop);
            ns = Value(weak<const Namespace>(newns), newns->metaclass);
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

void Namespace::registerClassRoot(weak<const ClassInfo> klass)
{
    Value v = Value(be_typeid<void>::klass());
    v["name"];
    weak<const Namespace> ns = getOrCreateNamespace(klass->name);
    be_assert_recover(ns, "impossible to create namespace %s" | klass->name, return);
    ref<const MetaPropertyInfo> prop = ref<const MetaPropertyInfo>::create(rttiArena(), Value(klass));
    ns->metaclass->addProperty(klass->name[klass->name.size()-1], prop);
    for(minitl::intrusive_list<const ClassInfo>::iterator it = klass->m_children.begin(); it != klass->m_children.end(); ++it)
    {
        registerClassRoot(it.operator->());
    }
}

}}
