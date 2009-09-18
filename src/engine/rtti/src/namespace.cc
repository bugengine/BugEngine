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


bool Namespace::MetaClass::Property::readable(Object* from) const
{
    UNUSED(from);
    return true;
}

bool Namespace::MetaClass::Property::writable(Object* from) const
{
    UNUSED(from);
    return true;
}

void Namespace::MetaClass::Property::set(Object* dest, const Value& value) const
{
    UNUSED(dest);
    m_value = value;
}

Value Namespace::MetaClass::Property::get(Object* from) const
{
    UNUSED(from);
    return m_value;
}

//-----------------------------------------------------------------------------

Namespace::MetaClass::MetaClass() :
    Object::MetaClass(inamespace("meta.namespace"), 0, 0, false)
{
}

Namespace::MetaClass::~MetaClass()
{
}

void Namespace::MetaClass::set(const istring& name, const Value& value)
{
    std::pair<PropertyIterator, bool> result = m_properties.insert(std::make_pair(name, refptr<Property>(0)));
    if(result.second)
        result.first->second = new Property();
    result.first->second->set(this, value);
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

Namespace::Namespace() :
    m_metaClass(new Namespace::MetaClass())
{
}

Namespace::~Namespace()
{
}

const Namespace::MetaClass* Namespace::metaclass() const
{
    return m_metaClass.get();
}

void Namespace::set(const istring& name, const Value& value)
{
    m_metaClass->set(name, value);
}

Value Namespace::get(const istring& name) const
{
    return m_metaClass->get(name);
}

void Namespace::mount(const istring& name, refptr<Namespace> ns)
{
    std::pair<NamespaceMap::iterator, bool> inserted = m_subnamespaces.insert(std::make_pair(name, refptr<Namespace>(ns)));
    be_assert(inserted.second, "could not insert namespace %s; a namespace of that name already exists" | name.c_str());
    if(! inserted.second)
        inserted.first->second = ns;
    m_metaClass->set(name, Value(refptr<Object>(ns)));
}

void Namespace::umount(const istring& name)
{
    m_subnamespaces.erase(name);
    m_metaClass->erase(name);
}

refptr<Namespace> Namespace::getNamespace(const istring& name)
{
    NamespaceMap::iterator it = m_subnamespaces.find(name);
    if(it != m_subnamespaces.end())
    {
        return it->second.get();
    }
    else
    {
        return 0;
    }
}

refptr<Namespace> Namespace::createNamespace(const istring& name)
{
    NamespaceMap::iterator it = m_subnamespaces.find(name);
    if(it != m_subnamespaces.end())
    {
        return it->second.get();
    }
    else
    {
        refptr<Namespace> ns = new Namespace();
        this->mount(name, ns);
        return ns;
    }
}

refptr<Namespace> Namespace::root()
{
    static refptr<Namespace> s_root = new Namespace;
    return s_root;
}

void Namespace::insert(const inamespace& ns, const Value& value)
{
    refptr<Namespace> _ns = this;
    for(size_t i = 0; i < ns.size() -1; ++i)
    {
        refptr<Namespace> child = _ns->getNamespace(ns[i]);
        if (!child)
        {
            child = new Namespace();
            _ns->mount(ns[i], child);
        }
        _ns = child;
    }
    _ns->set(ns[ns.size()-1], value);
}

void Namespace::insert(const inamespace& ns, Object* value)
{
    insert(ns, Value(value));
}

void Namespace::insert(const inamespace& ns, refptr<Object> value)
{
    insert(ns, Value(value));
}

Value Namespace::get(const inamespace& ns)
{
    refptr<Namespace> _ns = this;
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
    m_metaClass = 0;
}

}}
