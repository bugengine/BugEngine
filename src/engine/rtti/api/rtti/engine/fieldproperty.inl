/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_RTTI_ENGINE_FIELDPROPERTY_INL_
#define BE_RTTI_ENGINE_FIELDPROPERTY_INL_
/*****************************************************************************/

namespace BugEngine { namespace RTTI
{

template< typename Class, typename T >
FieldPropertyInfo<Class, T>::FieldPropertyInfo(T(Class::*member))
:   m_member(member)
{
}

template< typename Class, typename T >
FieldPropertyInfo<Class, T>::~FieldPropertyInfo()
{
}

template< typename Class, typename T >
const TypeInfo* FieldPropertyInfo<Class, T>::type() const
{
    return be_typeid<T>();
}

template< typename Class, typename T >
Value FieldPropertyInfo<Class, T>::get(void* from) const
{
    const Class* c = reinterpret_cast<const Class*>(from);
    return Value(c->*m_member);
}

template< typename Class, typename T >
void FieldPropertyInfo<Class, T>::set(const Value& v, void* to) const
{
    Class* c = reinterpret_cast<Class*>(to);
    c->*m_member = v.as<T>();
}

}}


/*****************************************************************************/
#endif
