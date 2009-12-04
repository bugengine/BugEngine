/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#include    <rtti/stdafx.h>

#include    <rtti/namespace.hh>

namespace BugEngine { namespace RTTI
{



Namespace::MetaClass::Property::Property() :
    m_value()
{
}

Namespace::MetaClass::Property::~Property()
{
}


bool Namespace::MetaClass::Property::readable(weak<Object> from) const
{
    UNUSED(from);
    return true;
}

bool Namespace::MetaClass::Property::writable(weak<Object> from) const
{
    UNUSED(from);
    return true;
}

void Namespace::MetaClass::Property::set(weak<Object> dest, const Value& value) const
{
    UNUSED(dest);
    m_value = value;
}

Value Namespace::MetaClass::Property::get(weak<Object> from) const
{
    UNUSED(from);
    return m_value;
}

//-----------------------------------------------------------------------------

Namespace::MetaClass::MetaClass()
: Object::MetaClass(inamespace("meta.namespace"), ref<RTTI::MetaClass>(), ref<RTTI::MetaMetaClass>(), false)
{
}

Namespace::MetaClass::~MetaClass()
{
}

weak<const RTTI::Property> Namespace::MetaClass::set(const istring& name, const Value& value)
{
    std::pair<PropertyIterator, bool> result = m_properties.insert(std::make_pair(name, ref<Property>()));
    if(result.second)
        result.first->second = ref<Property>::create();
    result.first->second->set(this, value);
    return result.first->second;
}

Value Namespace::MetaClass::get(const istring& name)
{
    PropertyIterator it = m_properties.find(name);
    if(it == m_properties.end())
        return Value();
    else
        return it->second->get(this);
}

void Namespace::MetaClass::erase(const istring& name)
{
    m_properties.erase(name);
}

//-----------------------------------------------------------------------------

Namespace::Namespace()
: m_metaClass(ref<Namespace::MetaClass>::create())
{
}

Namespace::~Namespace()
{
}

ref<const RTTI::MetaClass> Namespace::metaclass() const
{
    return m_metaClass;
}

weak<const RTTI::Property> Namespace::set(const istring& name, const Value& value)
{
    return m_metaClass->set(name, value);
}

Value Namespace::get(const istring& name) const
{
    return m_metaClass->get(name);
}

void Namespace::mount(const istring& name, ref<Namespace> ns)
{
    std::pair<NamespaceMap::iterator, bool> inserted = m_subnamespaces.insert(std::make_pair(name, ref<Namespace>(ns)));
    be_assert(inserted.second, "could not insert namespace %s; a namespace of that name already exists" | name.c_str());
    if(! inserted.second)
        inserted.first->second = ns;
    m_metaClass->set(name, Value(ref<Object>(ns)));
}

void Namespace::umount(const istring& name)
{
    m_subnamespaces.erase(name);
    m_metaClass->erase(name);
}

ref<Namespace> Namespace::getNamespace(const istring& name)
{
    NamespaceMap::iterator it = m_subnamespaces.find(name);
    if(it != m_subnamespaces.end())
    {
        return it->second;
    }
    else
    {
        return ref<Namespace>();
    }
}

ref<Namespace> Namespace::createNamespace(const istring& name)
{
    NamespaceMap::iterator it = m_subnamespaces.find(name);
    if(it != m_subnamespaces.end())
    {
        return it->second;
    }
    else
    {
        ref<Namespace> ns = ref<Namespace>::create();
        this->mount(name, ns);
        return ns;
    }
}

ref<Namespace> Namespace::root()
{
    Malloc::init();
    static ref<Namespace> s_root = ref<Namespace>::create();
    return s_root;
}

weak<const RTTI::Property> Namespace::insert(const inamespace& ns, const Value& value)
{
    weak<Namespace> _ns = this;
    for(size_t i = 0; i < ns.size() -1; ++i)
    {
        ref<Namespace> child = _ns->getNamespace(ns[i]);
        if (!child)
        {
            child = ref<Namespace>::create();
            _ns->mount(ns[i], child);
        }
        _ns = child;
    }
    return _ns->set(ns[ns.size()-1], value);
}

weak<const RTTI::Property> Namespace::insert(const inamespace& ns, weak<Object> value)
{
    return insert(ns, Value(value));
}

weak<const RTTI::Property> Namespace::insert(const inamespace& ns, ref<Object> value)
{
    return insert(ns, Value(value));
}

Value Namespace::get(const inamespace& ns)
{
    weak<Namespace> _ns = this;
    for(size_t i = 0; i < ns.size() -1; ++i)
    {
        _ns = _ns->getNamespace(ns[i]);
        if (!_ns)
            return Value();
    }
    return _ns->get(ns[ns.size()-1]);
}

void Namespace::clear()
{
    m_subnamespaces.clear();
    m_metaClass.clear();
}

}}
