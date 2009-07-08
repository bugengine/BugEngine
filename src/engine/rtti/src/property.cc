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

#include    <rtti/property.hh>

namespace BugEngine
{

Object::MetaClass::Property::MetaClass::MetaClass() :
    BugEngine::Object::MetaClass("meta.property", 0, true)
{
}

Object::MetaClass::Property::MetaClass::MetaClass(const inamespace& name, const MetaClass* parent, bool registerClass) :
    BugEngine::Object::MetaClass(name, parent, registerClass)
{
}

Object::MetaClass::Property::MetaClass::~MetaClass()
{
}

//-----------------------------------------------------------------------------

Object::MetaClass::Property::Property() :
    Object()
{
}

Object::MetaClass::Property::~Property()
{
}

const Object::MetaClass::Property::MetaClass* Object::MetaClass::Property::metaclass() const
{
    return static_metaclass();
}

const Object::MetaClass::Property::MetaClass* Object::MetaClass::Property::static_metaclass()
{
    static const Property::MetaClass*  s_metaclass = new Property::MetaClass();
    return s_metaclass;
}

}
