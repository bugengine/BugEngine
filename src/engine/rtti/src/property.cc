/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#include    <rtti/stdafx.h>

#include    <rtti/property.hh>

namespace BugEngine { namespace RTTI
{

Property::MetaClass::MetaClass()
:   RTTI::MetaClass("meta.property", 0, 0, true)
{
}

Property::MetaClass::MetaClass(const inamespace& name, const MetaClass* parent, bool registerClass) :
BugEngine::Object::MetaClass(name, parent, 0, registerClass)
{
}

Property::MetaClass::~MetaClass()
{
}

//-----------------------------------------------------------------------------

Property::Property() :
    Object()
{
}

Property::~Property()
{
}

const Property::MetaClass* Property::metaclass() const
{
    return static_metaclass();
}

const Property::MetaClass* Property::static_metaclass()
{
    static const Property::MetaClass*  s_metaclass = new Property::MetaClass();
    return s_metaclass;
}

}}
