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

template< typename T, typename OWNER, typename ValueGetter, typename ValueSetter >
ObjectProperty<T,OWNER,ValueGetter,ValueSetter>::MetaClass::MetaClass() :
    Property::MetaClass("meta.objectproperty", Property::static_metaclass(), false)
{
}

template< typename T, typename OWNER, typename ValueGetter, typename ValueSetter >
ObjectProperty<T,OWNER,ValueGetter,ValueSetter>::MetaClass::~MetaClass()
{
}

//-----------------------------------------------------------------------------

template< typename T, typename OWNER, typename ValueGetter, typename ValueSetter >
ObjectProperty<T,OWNER,ValueGetter,ValueSetter>::ObjectProperty()
{
}

template< typename T, typename OWNER, typename ValueGetter, typename ValueSetter >
ObjectProperty<T,OWNER,ValueGetter,ValueSetter>::~ObjectProperty()
{
}

template< typename T, typename OWNER, typename ValueGetter, typename ValueSetter >
bool ObjectProperty<T,OWNER,ValueGetter,ValueSetter>::readable(Object* from) const
{
    UNUSED(from);
    return !minitl::type_equals< ValueGetter,GetImpossible<T,OWNER> >::Value;
}

template< typename T, typename OWNER, typename ValueGetter, typename ValueSetter >
bool ObjectProperty<T,OWNER,ValueGetter,ValueSetter>::writable(Object* from) const
{
    UNUSED(from);
    return !minitl::type_equals< ValueGetter,SetImpossible<T,OWNER> >::Value;
}

template< typename T, typename OWNER, typename ValueGetter, typename ValueSetter >
void ObjectProperty<T,OWNER,ValueGetter,ValueSetter>::set(Object* dest, const Value& value) const
{
    ValueSetter::set(checked_cast<OWNER*>(dest), m_marshaller.castto(value));
}

template< typename T, typename OWNER, typename ValueGetter, typename ValueSetter >
Value ObjectProperty<T,OWNER,ValueGetter,ValueSetter>::get(Object* from) const
{
    return ValueGetterProxy< ValueGetter,ValueGetter::RefPossible, Marshaller<T> >::get(checked_cast<OWNER*>(from),&m_marshaller);
}

template< typename T, typename OWNER, typename ValueGetter, typename ValueSetter >
const typename ObjectProperty<T,OWNER,ValueGetter,ValueSetter>::MetaClass* ObjectProperty<T,OWNER,ValueGetter,ValueSetter>::metaclass() const
{
    return static_metaclass();
}

template< typename T, typename OWNER, typename ValueGetter, typename ValueSetter >
const typename ObjectProperty<T,OWNER,ValueGetter,ValueSetter>::MetaClass* ObjectProperty<T,OWNER,ValueGetter,ValueSetter>::static_metaclass()
{
    static typename ObjectProperty<T,OWNER,ValueGetter,ValueSetter>::MetaClass* s_metaclass = new typename ObjectProperty<T,OWNER,ValueGetter,ValueSetter>::MetaClass;
    return s_metaclass;
}


}}

/*****************************************************************************/
#endif
