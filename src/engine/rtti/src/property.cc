/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#include    <rtti/stdafx.h>

#include    <rtti/property.hh>

namespace BugEngine { namespace RTTI
{

Property::MetaClass::MetaClass()
:   RTTI::MetaClass("meta.property", ref<RTTI::MetaClass>(), ref<RTTI::MetaMetaClass>(), true)
{
}

Property::MetaClass::MetaClass(const inamespace& name, ref<const MetaClass> parent, bool registerClass)
:    BugEngine::Object::MetaClass(name, parent, ref<MetaMetaClass>(), registerClass)
{
}

Property::MetaClass::~MetaClass()
{
}

//-----------------------------------------------------------------------------

Property::Property()
:   Object()
{
}

Property::~Property()
{
}

ref<const RTTI::MetaClass> Property::metaclass() const
{
    return static_metaclass();
}

ref<const Property::MetaClass> Property::static_metaclass()
{
    static ref<const Property::MetaClass> s_metaclass = ref<Property::MetaClass>::create();
    return s_metaclass;
}

}}
