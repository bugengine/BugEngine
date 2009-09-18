/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#include    <rtti/stdafx.h>

#include    <rtti/method.hh>
#include    <rtti/property.hh>

namespace BugEngine { namespace RTTI
{

Method::MetaClass::MetaClass()
    :   RTTI::MetaClass("meta.method", 0, 0, true)
{
}

Method::MetaClass::MetaClass(const inamespace& name, const MetaClass* parent, bool registerClass)
    :   RTTI::MetaClass(name, parent, 0, registerClass)
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

const Method::MetaClass* Method::metaclass() const
{
    return static_metaclass();
}

const Method::MetaClass* Method::static_metaclass()
{
    static const Method::MetaClass* s_metaclass = new Method::MetaClass;
    return s_metaclass;
}


}}
