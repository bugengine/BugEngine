/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_MINITL_TYPE_CAST_
#define BE_MINITL_TYPE_CAST_
/*****************************************************************************/
#include    <core/debug/assert.hh>
#include    <typeinfo>
#include    <minitl/ptr/refptr.hh>

namespace minitl
{

template< typename U, typename T >
inline U* be_checked_cast(T* value)
{
    be_assert(dynamic_cast<U*>(value), "invalid cast from %s to %s" | typeid(T).name() | typeid(U).name());
    return static_cast<U*>(value);
}

template< typename U, typename T >
inline ref<U> be_checked_cast(ref<T> value)
{
    be_assert(dynamic_cast<U*>(value.operator->()), "invalid cast from %s* to %s*" | typeid(T).name() | typeid(U).name());
    return ref<U>(static_cast<U*>(value.operator->()));
}

template< typename U, typename T >
inline weak<U> be_checked_cast(weak<T> value)
{
    be_assert(dynamic_cast<U*>(value.operator->()), "invalid cast from %s* to %s*" | typeid(T).name() | typeid(U).name());
    return weak<U>(static_cast<U*>(value.operator->()));
}

template< typename U, typename T >
inline weak<U> be_const_cast(weak<T> value)
{
    return weak<U>(const_cast<U*>(value.operator->()));
}

template< typename U, typename T >
inline ref<U> be_const_cast(ref<T> value)
{
    return ref<U>(const_cast<U*>(value.operator->()));
}

#if BE_COMPILER_MSVC
# pragma warning(push)
# pragma warning(disable:4800)
# pragma warning(disable:4267)
#endif

template< typename U, typename T >
inline U be_checked_numcast(T value)
{
    be_assert(static_cast<T>(static_cast<U>(value)) == value, "precision loss during cast from %s to %s" | typeid(T).name() | typeid(U).name());
    return static_cast<U>(value);
}

#if BE_COMPILER_MSVC
# pragma warning(pop)
#endif
}

/*****************************************************************************/
#endif
