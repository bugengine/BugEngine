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

ref<const Object::MetaClass> Object::static_metaclass()
{
    ref<MetaClass::MetaMetaClass> metametaclass = ref<MetaClass::MetaMetaClass>::create("ObjectClass", ref<MetaClass::MetaMetaClass>());
    static ref<Object::MetaClass> s_metaclass = ref<Object::MetaClass>::create("Object", ref<MetaClass>(), metametaclass, true);
    return s_metaclass;
}

void Object::registerMetaClass()
{
    static_metaclass();
}

}
