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

#include    <data/stdafx.h>
#include    <parse/type.hh>
#include    <parse/context.hh>

namespace BugEngine { namespace Data { namespace Parse
{

Type::Type()
{
}

Type::~Type()
{
}

//-----------------------------------------------------------------------------

Type::Any::Any()
{
}

Type::Any::~Any()
{
}

void Type::Any::dolink(Context& /*context*/) const
{
}

Value Type::Any::doeval(Context& /*context*/) const
{
    return Value();
}

//-----------------------------------------------------------------------------

Type::Bool::Bool()
{
}

Type::Bool::~Bool()
{
}

void Type::Bool::dolink(Context& /*context*/) const
{
}

Value Type::Bool::doeval(Context& /*context*/) const
{
    return Value();
}

//-----------------------------------------------------------------------------

Type::Int::Int()
{
}

Type::Int::~Int()
{
}

void Type::Int::dolink(Context& /*context*/) const
{
}

Value Type::Int::doeval(Context& /*context*/) const
{
    return Value();
}

//-----------------------------------------------------------------------------

Type::Float::Float()
{
}

Type::Float::~Float()
{
}

void Type::Float::dolink(Context& /*context*/) const
{
}

Value Type::Float::doeval(Context& /*context*/) const
{
    return Value();
}

//-----------------------------------------------------------------------------

Type::String::String()
{
}

Type::String::~String()
{
}

void Type::String::dolink(Context& /*context*/) const
{
}

Value Type::String::doeval(Context& /*context*/) const
{
    return Value();
}

//-----------------------------------------------------------------------------

Type::Vector::Vector(refptr<Type> cellType) :
    m_cellType(cellType)
{
}

Type::Vector::~Vector()
{
}

void Type::Vector::dolink(Context& /*context*/) const
{
}

Value Type::Vector::doeval(Context& /*context*/) const
{
    return Value();
}

//-----------------------------------------------------------------------------

Type::Map::Map(refptr<Type> keyType, refptr<Type> valueType) :
    m_keyType(keyType),
    m_valueType(valueType)
{
}

Type::Map::~Map()
{
}

void Type::Map::dolink(Context& /*context*/) const
{
}

Value Type::Map::doeval(Context& /*context*/) const
{
    return Value();
}

}}}
