/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#include    <rtti/stdafx.h>

#include    <rtti/method.hh>
#include    <rtti/property.hh>

namespace BugEngine { namespace RTTI
{

Method::MetaClass::MetaClass()
:   RTTI::MetaClass("meta.method", ref<RTTI::MetaClass>(), ref<RTTI::MetaMetaClass>(), true)
{
}

Method::MetaClass::MetaClass(const inamespace& name, ref<const MetaClass> parent, bool registerClass)
    :   RTTI::MetaClass(name, parent, ref<MetaMetaClass>(), registerClass)
{
}

Method::MetaClass::~MetaClass()
{
}

//-----------------------------------------------------------------------------

Method::Method()
{
}

Method::~Method()
{
}

ref<const RTTI::MetaClass> Method::metaclass() const
{
    return static_metaclass();
}

ref<const Method::MetaClass> Method::static_metaclass()
{
    static ref<const Method::MetaClass> s_metaclass = ref<Method::MetaClass>::create();
    return s_metaclass;
}


}}
