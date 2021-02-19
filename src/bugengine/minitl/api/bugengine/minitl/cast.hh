/* BugEngine <bugengine.devel@gmail.com>
   see LICENSE for detail */

#ifndef BE_MINITL_CAST_HH_
#define BE_MINITL_CAST_HH_
/**************************************************************************************************/
#include <bugengine/minitl/stdafx.h>

namespace minitl {

template < typename T >
class ref;
template < typename T >
class weak;

template < typename U, typename T >
inline U* be_checked_cast(T* value);

template < typename U, typename T >
inline ref< U > be_checked_cast(ref< T > value);

template < typename U, typename T >
inline weak< U > be_checked_cast(weak< T > value);

template < typename U, typename T >
inline weak< U > be_const_cast(weak< T > value);

template < typename U, typename T >
inline ref< U > be_const_cast(ref< T > value);

template < typename U, typename T >
inline U be_function_cast(T value);

template < typename U, typename T >
inline U be_checked_numcast(T value);

}  // namespace minitl

#include <bugengine/minitl/cast.inl>

/**************************************************************************************************/
#endif
