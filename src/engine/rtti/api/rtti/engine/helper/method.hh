/* BugEngine / 2008-2012  Nicolas MERCIER <mercier.nicolas@gmail.com>
   see LICENSE for detail */

#ifndef BE_RTTI_ENGINE_HELPER_METHOD_HH_
#define BE_RTTI_ENGINE_HELPER_METHOD_HH_
/*****************************************************************************/
#include   <rtti/value.inl>

namespace BugEngine { namespace RTTI
{

template< typename T >
static inline Value call(Value& _this, Value* params, u32 paramCount)
{
    return _this.as<const T&>()(params, paramCount);
}

template< typename T >
static inline void wrapCopy(const void* src, void* dst)
{
    new(dst) T(*(T*)src);
}

template< typename T >
static inline void wrapDestroy(void* src)
{
    be_forceuse(src);
    ((T*)src)->~T();
}

template< size_t size >
inline void nullconstructor(const void* src, void* dst)
{
    memcpy(dst, src, size);
}

template< >
inline void nullconstructor<0>(const void* /*src*/, void* /*dst*/)
{
}

static inline void nulldestructor(void*)
{
}

template< typename T, Value(T::*call)(Value*, u32) >
static Value wrapCall(Value* params, u32 paramCount)
{
    be_assert_recover(be_typeid<T*>::type() <= params[0].type(), "expected parameter of type %s; got %s" | be_typeid<T*>::type() | params[0].type(), return Value());
    return (params[0].as<T&>().*call)(params + 1, paramCount - 1);
}

template< typename T, Value(T::*call)(Value*, u32) const >
static Value wrapCallConst(Value* params, u32 paramCount)
{
    be_assert_recover(be_typeid<const T*>::type() <= params[0].type(), "expected parameter of type %s; got %s" | be_typeid<const T*>::type() | params[0].type(), return Value());
    return (params[0].as<const T&>().*call)(params + 1, paramCount - 1);
}

}}

/*****************************************************************************/
#endif
