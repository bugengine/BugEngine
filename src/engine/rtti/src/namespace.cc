/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#include    <rtti/stdafx.h>
#include    <rtti/typeinfo.hh>
#include    <rtti/typeinfo.inl>
#include    <rtti/namespace.script.hh>
#include    <rtti/classinfo.script.hh>
#include    <rtti/engine/propertyinfo.script.hh>
#include    <rtti/engine/methodinfo.script.hh>
#include    <rtti/value.inl>

namespace BugEngine { namespace RTTI
{

Namespace::Namespace()
    :   decls(rttiArena())
{
}

Namespace::~Namespace()
{
}

weak<const Namespace> Namespace::getNamespace(const inamespace& name, CreationPolicy policy)const 
{
    Value ns = Value(weak<const Namespace>(this));
    for (size_t i = 0; i < name.size()-1; ++i)
    {
        Value v = ns[name[i]];
        weak<const Namespace> nsobj = ns.as< weak<const Namespace> >();
        if (!v)
        {
            if (policy == Create)
            {
                ref<Namespace> newns = ref<Namespace>::create(rttiArena());
                nsobj->add(name[i], Value(newns));
                ns = Value(weak<const Namespace>(newns));
            }
            else
            {
                return weak<const Namespace>();
            }
        }
        else
        {
            ns = v;
            if (! (be_typeid< weak<const Namespace> >::type() <= ns.type()))
            {
                be_error("Unable to convert value %s of type %s to a Namespace" | name | ns.type().name());
                return weak<const Namespace>();
            }
        }
    }

    return ns.as< weak<const Namespace> >();
}

void Namespace::add(const inamespace& name, const Value& value) const
{
    weak<const Namespace> ns = getNamespace(name, Create);
    if (ns)
    {
        ns->add(name[name.size()-1], value);
    }
}

void Namespace::add(const istring& name, const Value& value) const
{
    decls.insert(std::make_pair(name, value));
}

void Namespace::remove(const inamespace& name) const
{
    weak<const Namespace> ns = getNamespace(name, DoNotCreate);
    if (ns)
    {
        ns->remove(name[name.size()-1]);
        if (ns->empty() && name.size() > 1)
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
    decls.erase(name);
}

Value Namespace::get(const istring& name) const
{
    minitl::hashmap<istring, Value>::const_iterator it = decls.find(name);
    if (it != decls.end())
        return it->second;
    else
        return Value();
}

void Namespace::clearNamespace()
{
    decls.clear();
}

}


weak<const RTTI::Namespace> be_Namespace()
{
    be_typeid<RTTI::Namespace>::klass();
    static ref<RTTI::Namespace> s_root = ref<RTTI::Namespace>::create(rttiArena());
    return s_root;
}

}
