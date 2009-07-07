/*****************************************************************************\
* BugEngine                                                                   *
* Copyright (C) 2005-2008  screetch <screetch@gmail.com>                      *
*                                                                             *
* This library is free software; you can redistribute it and/or modify it     *
* under the terms of the GNU Lesser General Public License as published by    *
* the Free Software Foundation; either version 2.1 of the License, or (at     *
* your option) any later version.                                             *
*                                                                             *
* This library is distributed in the hope that it will be useful, but WITHOUT *
* ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or       *
* FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public        *
* License for more details.                                                   *
*                                                                             *
* You should have received a copy of the GNU Lesser General Public License    *
* along with this library; if not, write to                                   *
* the Free Software Foundation, Inc.,                                         *
* 51 Franklin St,                                                             *
* Fifth Floor,                                                                *
* Boston, MA 02110-1301                                                       *
* USA                                                                         *
\*****************************************************************************/

#include    <rtti/stdafx.h>

#include    <rtti/metaclass.hh>
#include    <rtti/property.hh>
#include    <rtti/namespace.hh>

namespace BugEngine
{

class Object::MetaClass::MetaMetaClass : public MetaClass
{
public:
    MetaMetaClass();
    ~MetaMetaClass();
private:
    MetaMetaClass(const MetaMetaClass& other);
    MetaMetaClass& operator=(const MetaMetaClass& other);
};

Object::MetaClass::MetaMetaClass::MetaMetaClass() :
    MetaClass("meta.metaclass", 0)
{
}

Object::MetaClass::MetaMetaClass::~MetaMetaClass()
{
}

//-----------------------------------------------------------------------------

Object::MetaClass::MetaMetaClass* Object::MetaClass::s_metaMetaClass = 0;

Object::MetaClass::MetaClass(const inamespace& name, const MetaClass* parent, bool registerClass) :
    m_name(name[name.size()-1]),
    m_parent(parent),
    m_properties()
{
    if(parent)
        m_properties = parent->m_properties;
    if(registerClass)
    {
        Namespace::root()->insert(name, Value(static_cast< refptr<Object> >(this)));
    }
}

Object::MetaClass::~MetaClass()
{
}

const istring& Object::MetaClass::name() const
{
    return m_name;
}

const Object::MetaClass* Object::MetaClass::metaclass() const
{
    if(! s_metaMetaClass)
        s_metaMetaClass = new MetaMetaClass();
    return s_metaMetaClass;
}

const Object::MetaClass* Object::MetaClass::parent() const
{
    return m_parent.get();
}

void Object::MetaClass::addProperty(const istring& name, refptr<const Property> prop)
{
    std::pair<PropertyIterator,bool> result = m_properties.insert(std::make_pair(name, prop));
    Assert(result.second);
}

const Property* Object::MetaClass::getProperty(const istring& name) const
{
    PropertyConstIterator it = m_properties.find(name);
    if(it != m_properties.end())
        return it->second.get();
    else
        return 0;
}

void Object::MetaClass::init(MetaClass* mc)
{
}

}
