/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_RTTI_VALUECOPY_INL_
#define BE_RTTI_VALUECOPY_INL_
/*****************************************************************************/

namespace BugEngine { namespace RTTI
{

template< typename T >
inline ValueCopy::ValueCopy(const T& value) :
    m_value(value)
{
}

template< typename T >
inline void ValueCopy::operator=(const T& value)
{
    m_value = value;
}

template< typename T >
inline T ValueCopy::as() const
{
    return m_value.as<T>();
}

template< typename T >
inline T ValueCopy::member() const
{
    return m_value.as<T>();
}


inline ValueTypeIndex ValueCopy::typeindex() const
{
    return ValueTypeIndex(m_value.typeindex());
}


}}


/*****************************************************************************/
#endif
