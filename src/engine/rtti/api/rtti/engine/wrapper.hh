/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_RTTI_ENGINE_WRAPPER_HH_
#define BE_RTTI_ENGINE_WRAPPER_HH_
/*****************************************************************************/
#include   <rtti/value.hh>

namespace BugEngine { namespace RTTI
{

template< typename T, typename Owner, T (Owner::*Field) >
static inline Value get(Value& from)
{
    if(from.isConst())
    {
        return Value(Value::ByRef(from.as<const Owner>().*Field));
    }
    else
    {
        return Value(Value::ByRef(from.as<Owner>().*Field));
    }
}

template< typename T, typename Owner, T (Owner::*Field) >
static inline void set(Value& object, Value& value)
{
    from.as<Owner>().*Field = value.as<T>();
}

}}

/*****************************************************************************/
#endif
