/* BugEngine <bugengine.devel@gmail.com>
   see LICENSE for detail */

#ifndef BE_MINITL_CAST_INL_
#define BE_MINITL_CAST_INL_
/**************************************************************************************************/
#include <bugengine/minitl/stdafx.h>
#include <bugengine/minitl/assert.hh>
#include <bugengine/minitl/features.hh>
#include <bugengine/minitl/refptr.hh>
#include <bugengine/minitl/weakptr.hh>
#if BE_ENABLE_ASSERT
#    include <typeinfo>
#endif

namespace minitl {

template < typename U, typename T >
inline U* be_checked_cast(T* value)
{
    be_assert(!value || dynamic_cast< U* >(value), "invalid cast from %s to %s, actual type %s"
                                                       | typeid(T).name() | typeid(U).name()
                                                       | typeid(*value).name());
    return static_cast< U* >(value);
}

template < typename U, typename T >
inline ref< U > be_checked_cast(ref< T > value)
{
    be_assert(!value || dynamic_cast< U* >(value.operator->()),
              "invalid cast from ref<%s> to ref<%s>, actual type ref<%s>" | typeid(T).name()
                  | typeid(U).name() | typeid(*value.operator->()).name());
    return ref< U >(static_cast< U* >(value.operator->()));
}

template < typename U, typename T >
inline weak< U > be_checked_cast(weak< T > value)
{
    be_assert(!value || dynamic_cast< U* >(value.operator->()),
              "invalid cast from weak<%s> to weak<%s>, actual type weak<%s>" | typeid(T).name()
                  | typeid(U).name() | typeid(*value.operator->()).name());
    return weak< U >(static_cast< U* >(value.operator->()));
}

template < typename U, typename T >
inline weak< U > be_const_cast(weak< T > value)
{
    return weak< U >(const_cast< U* >(value.operator->()));
}

template < typename U, typename T >
inline ref< U > be_const_cast(ref< T > value)
{
    return ref< U >(const_cast< U* >(value.operator->()));
}

template < typename U, typename T >
inline U be_function_cast(T value)
{
    typedef void (*GenericFunction)();
    return reinterpret_cast< U >(reinterpret_cast< GenericFunction >(value));
}

#if BE_COMPILER_MSVC
#    pragma warning(push)
#    pragma warning(disable : 4800)
#    pragma warning(disable : 4267)
#endif

template < typename U, typename T >
inline U be_checked_numcast(T value)
{
    be_assert(static_cast< T >(static_cast< U >(value)) == value,
              "precision loss during cast from %s to %s" | typeid(T).name() | typeid(U).name());
    return static_cast< U >(value);
}

#if BE_COMPILER_MSVC
#    pragma warning(pop)
#endif
}  // namespace minitl

/**************************************************************************************************/
#endif
