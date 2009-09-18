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
BE_ALWAYSINLINE U checked_cast(T value)
{
    be_assert(dynamic_cast<U>(value), "invalid cast from %s to %s" | typeid(T).name() | typeid(U).name());
    return static_cast<U>(value);
}

template< typename U, typename T >
BE_ALWAYSINLINE refptr<U> checked_cast(refptr<T> value)
{
    be_assert(dynamic_cast<U*>(value.get()), "invalid cast from %s* to %s*" | typeid(T).name() | typeid(U).name());
    return refptr<U>(static_cast<U*>(value.get()));
}

template< typename U, typename T >
BE_ALWAYSINLINE weakptr<U> checked_cast(weakptr<T> value)
{
    be_assert(dynamic_cast<U*>(value.get()), "invalid cast from %s* to %s*" | typeid(T).name() | typeid(U).name());
    return weakptr<U>(checked_cast<U>(refptr<T>(value.get())));
}

#pragma warning(push)
#pragma warning(disable:4800)

template< typename U, typename T >
BE_ALWAYSINLINE U checked_numcast(T value)
{
    be_assert(static_cast<T>(static_cast<U>(value)) == value, "precision loss during cast from %s to %s" | typeid(T).name() | typeid(U).name());
    return static_cast<U>(value);
}

#pragma warning(pop)

}

/*****************************************************************************/
#endif
