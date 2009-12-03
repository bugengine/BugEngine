/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_RTTI_OBJECTPROPERTY_INL_
#define BE_RTTI_OBJECTPROPERTY_INL_
/*****************************************************************************/
#include    <minitl/type/typemanipulation.hh>

namespace BugEngine { namespace RTTI
{

template< typename OWNER, typename T, typename ValueGetter, typename ValueSetter >
ObjectProperty<OWNER,T,ValueGetter,ValueSetter>::MetaClass::MetaClass()
:   Property::MetaClass("meta.objectproperty", Property::static_metaclass(), false)
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
bool ObjectProperty<OWNER,T,ValueGetter,ValueSetter>::readable(weak<Object> from) const
{
    UNUSED(from);
    return bool(ValueGetter::Read);
}

template< typename OWNER, typename T, typename ValueGetter, typename ValueSetter >
bool ObjectProperty<OWNER,T,ValueGetter,ValueSetter>::writable(weak<Object> from) const
{
    UNUSED(from);
    return bool(ValueSetter::Write);
}

template< typename OWNER, typename T, typename ValueGetter, typename ValueSetter >
void ObjectProperty<OWNER,T,ValueGetter,ValueSetter>::set(weak<Object> dest, const Value& value) const
{
    ValueSetter::set(be_checked_cast<OWNER>(dest), m_marshaller.castto(value));
}

template< typename OWNER, typename T, typename ValueGetter, typename ValueSetter >
Value ObjectProperty<OWNER,T,ValueGetter,ValueSetter>::get(weak<Object> from) const
{
    return ValueGetterProxy< ValueGetter,ValueGetter::Ref, Marshaller<T> >::get(be_checked_cast<OWNER>(from),&m_marshaller);
}

template< typename OWNER, typename T, typename ValueGetter, typename ValueSetter >
ref<const RTTI::MetaClass> ObjectProperty<OWNER,T,ValueGetter,ValueSetter>::metaclass() const
{
    return static_metaclass();
}

template< typename OWNER, typename T, typename ValueGetter, typename ValueSetter >
ref<const typename ObjectProperty<OWNER,T,ValueGetter,ValueSetter>::MetaClass> ObjectProperty<OWNER,T,ValueGetter,ValueSetter>::static_metaclass()
{
    static ref<typename ObjectProperty<OWNER,T,ValueGetter,ValueSetter>::MetaClass> s_metaclass = ref<typename ObjectProperty<OWNER,T,ValueGetter,ValueSetter>::MetaClass>::create();
    return s_metaclass;
}


}}

/*****************************************************************************/
#endif
