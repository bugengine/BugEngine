/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_RTTI_ENGINE_HELPER_GET_HH_
#define BE_RTTI_ENGINE_HELPER_GET_HH_
/*****************************************************************************/
#include   <rtti/value.inl>

namespace BugEngine { namespace RTTI
{

template< typename T, typename Owner, T (Owner::*Field) >
static inline Value get(weak<const PropertyInfo> _this, void* from, bool isConst)
{
    if(isConst)
    {
        return Value(Value::ByRef((const Owner*)from->*Field));
    }
    else
    {
        return Value(Value::ByRef((Owner*)from->*Field));
    }
}

template< typename T, T v >
static inline Value staticgetvalue(weak<const PropertyInfo> _this, void* from, bool isConst)
{
    return Value(v);
}

template< typename T, T* object >
static inline Value staticgetobject(weak<const PropertyInfo> _this, void* from, bool isConst)
{
    return Value(*object);
}

}}

/*****************************************************************************/
#endif
