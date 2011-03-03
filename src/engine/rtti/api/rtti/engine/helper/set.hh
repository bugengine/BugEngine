/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_RTTI_ENGINE_HELPER_SET_HH_
#define BE_RTTI_ENGINE_HELPER_SET_HH_
/*****************************************************************************/
#include   <rtti/value.hh>

namespace BugEngine { namespace RTTI
{

template< typename T, typename Owner, T (Owner::*Field) >
static inline void set(weak<const PropertyInfo> _this, void* from, Value& value, bool isConst)
{
    be_assert_recover(!isConst, "Setting property on const object", return);
    (Owner*)from->*Field = value.as<T>();
}

}}

/*****************************************************************************/
#endif
