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
#include    <rtti/method.hh>
#include    <rtti/namespace.hh>
#include    <rtti/helper.hh>

namespace BugEngine { namespace RTTI
{

class MetaClass::StaticProperty : public Property
{
private:
    Value   m_value;
public:
    StaticProperty(Value v);
    ~StaticProperty();

    virtual bool                readable(Object* /*from*/) const                    { return true; };
    virtual bool                writable(Object* /*from*/) const                    { return false; };
    virtual void                set(Object* /*dest*/, const Value& /*value*/) const { be_notreached(); }
    virtual Value               get(Object* /*from*/) const                         { return m_value; }
};

MetaClass::StaticProperty::StaticProperty(Value v)
    :   m_value(v)
{
}

MetaClass::StaticProperty::~StaticProperty()
{
}

//-----------------------------------------------------------------------------

MetaMetaClass::MetaMetaClass(const inamespace& name, const Object::MetaClass* parent)
    :   MetaClass(name, parent, 0, false)
{
    if(!parent)
    {
        addMethod("name", BugEngine::RTTI::_::createMethodBuilder(&Object::MetaClass::name).buildMethod<&Object::MetaClass::name>());
        addMethod("create", BugEngine::RTTI::_::createMethodBuilder(&Object::MetaClass::create).buildMethod<&Object::MetaClass::create>());
    }
}

MetaMetaClass::~MetaMetaClass()
{
}

//-----------------------------------------------------------------------------

MetaClass::MetaClass(const inamespace& name, const MetaClass* parent, MetaMetaClass* mc, bool registerClass)
    :   m_name(name[name.size()-1])
    ,   m_parent(parent)
    ,   m_metaclass(mc)
    ,   m_properties()
{
    if(parent)
        m_properties = parent->m_properties;
    else if(m_metaclass)
        addMethod("metaclass", BugEngine::RTTI::_::createMethodBuilder(&Object::metaclass).buildMethod<&Object::metaclass>());

    if(registerClass)
    {
        Namespace::root()->insert(name, Value(static_cast< refptr<Object> >(this)));
    }
}

MetaClass::~MetaClass()
{
}

const istring& MetaClass::name() const
{
    return m_name;
}

const MetaClass* MetaClass::metaclass() const
{
    return m_metaclass.get();
}

const MetaClass* MetaClass::parent() const
{
    return m_parent.get();
}

void MetaClass::addProperty(const istring& name, refptr<const Property> prop)
{
    std::pair<PropertyIterator,bool> result = m_properties.insert(std::make_pair(name, prop));
    be_assert(result.second, "could not register property %s; a property of that name already exists" | name.c_str());
}

void MetaClass::addMethod(const istring& name, refptr<Method> method)
{
    addProperty(name, new StaticProperty(Value(refptr<Object>(method))));
}

const Property* Object::MetaClass::getProperty(const istring& name) const
{
    PropertyConstIterator it = m_properties.find(name);
    if(it != m_properties.end())
        return it->second.get();
    else
        return 0;
}

Value MetaClass::call(Value* /*params*/, size_t /*nbParams*/) const
{
    be_notreached();
    return Value();
}

void MetaClass::init(MetaClass* /*mc*/)
{
}

refptr<Object> MetaClass::create() const
{
    be_notreached();
    return 0;
}

}}
