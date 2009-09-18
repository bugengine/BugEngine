/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#include    <rtti/stdafx.h>

#include    <rtti/object.hh>
#include    <rtti/metaclass.hh>
#include    <rtti/property.hh>

namespace BugEngine
{

Object::Object()
{
}

Object::~Object()
{
}

const Object::MetaClass* Object::static_metaclass()
{
    Malloc::init();
    static refptr<Object::MetaClass> s_metaclass(new Object::MetaClass("Object", 0, new MetaClass::MetaMetaClass("ObjectClass", 0), true));
    return s_metaclass.get();
}

void Object::registerMetaClass()
{
    static_metaclass();
}

}
