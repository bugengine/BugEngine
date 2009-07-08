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

#include    <rtti/method.hh>
#include    <rtti/property.hh>

namespace BugEngine
{

Object::MetaClass::Method::MetaClass::MetaClass() :
    BugEngine::Object::MetaClass("meta.method", 0, 0, true)
{
}

Object::MetaClass::Method::MetaClass::MetaClass(const inamespace& name, const MetaClass* parent, bool registerClass) :
    BugEngine::Object::MetaClass(name, parent, 0, registerClass)
{
}

Object::MetaClass::Method::MetaClass::~MetaClass()
{
}

//-----------------------------------------------------------------------------

Object::MetaClass::Method::Method()
{
}

Object::MetaClass::Method::~Method()
{
}

const Object::MetaClass::Method::MetaClass* Object::MetaClass::Method::metaclass() const
{
    return static_metaclass();
}

const Object::MetaClass::Method::MetaClass* Object::MetaClass::Method::static_metaclass()
{
    static const Method::MetaClass* s_metaclass = new Method::MetaClass;
    return s_metaclass;
}


}
