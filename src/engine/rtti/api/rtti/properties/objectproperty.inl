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

#ifndef BE_RTTI_OBJECTPROPERTY_INL_
#define BE_RTTI_OBJECTPROPERTY_INL_
/*****************************************************************************/
#include    <minitl/type/typemanipulation>

namespace BugEngine { namespace RTTI
{

template< typename OWNER, typename T, typename ValueGetter, typename ValueSetter >
ObjectProperty<OWNER,T,ValueGetter,ValueSetter>::MetaClass::MetaClass() :
    Property::MetaClass("meta.objectproperty", Property::static_metaclass(), false)
{
}

template< typename OWNER, typename T, typename ValueGetter, typename ValueSetter >
ObjectProperty<OWNER,T,ValueGetter,ValueSetter>::MetaClass::~MetaClass()
{
}

//-----------------------------------------------------------------------------

template< typename OWNER, typename T, typename ValueGetter, typename ValueSetter >
ObjectProperty<OWNER,T,ValueGetter,ValueSetter>::ObjectProperty()
{
}

template< typename OWNER, typename T, typename ValueGetter, typename ValueSetter >
ObjectProperty<OWNER,T,ValueGetter,ValueSetter>::~ObjectProperty()
{
}

template< typename OWNER, typename T, typename ValueGetter, typename ValueSetter >
bool ObjectProperty<OWNER,T,ValueGetter,ValueSetter>::readable(Object* from) const
{
    UNUSED(from);
    return bool(ValueGetter::Read);
}

template< typename OWNER, typename T, typename ValueGetter, typename ValueSetter >
bool ObjectProperty<OWNER,T,ValueGetter,ValueSetter>::writable(Object* from) const
{
    UNUSED(from);
    return bool(ValueSetter::Write);
}

template< typename OWNER, typename T, typename ValueGetter, typename ValueSetter >
void ObjectProperty<OWNER,T,ValueGetter,ValueSetter>::set(Object* dest, const Value& value) const
{
    ValueSetter::set(checked_cast<OWNER*>(dest), m_marshaller.castto(value));
}

template< typename OWNER, typename T, typename ValueGetter, typename ValueSetter >
Value ObjectProperty<OWNER,T,ValueGetter,ValueSetter>::get(Object* from) const
{
    return ValueGetterProxy< ValueGetter,ValueGetter::Ref, Marshaller<T> >::get(checked_cast<OWNER*>(from),&m_marshaller);
}

template< typename OWNER, typename T, typename ValueGetter, typename ValueSetter >
const typename ObjectProperty<OWNER,T,ValueGetter,ValueSetter>::MetaClass* ObjectProperty<OWNER,T,ValueGetter,ValueSetter>::metaclass() const
{
    return static_metaclass();
}

template< typename OWNER, typename T, typename ValueGetter, typename ValueSetter >
const typename ObjectProperty<OWNER,T,ValueGetter,ValueSetter>::MetaClass* ObjectProperty<OWNER,T,ValueGetter,ValueSetter>::static_metaclass()
{
    static typename ObjectProperty<OWNER,T,ValueGetter,ValueSetter>::MetaClass* s_metaclass = new typename ObjectProperty<OWNER,T,ValueGetter,ValueSetter>::MetaClass;
    return s_metaclass;
}


}}

/*****************************************************************************/
#endif
