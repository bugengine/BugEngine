/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
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
    ((T*)src)->~T();
}

template< size_t size >
static inline void nullconstructor(const void* src, void* dst) { memcpy(dst, src, size); }
static inline void nulldestructor(void*) { }


template<   typename T,
            typename R,
            typename P1 = void,
            typename P2 = void,
            typename P3 = void,
            typename P4 = void,
            typename P5 = void,
            typename P6 = void,
            typename P7 = void,
            typename P8 = void,
            typename P9 = void >
struct callhelper;


}}

#include    <rtti/engine/helper/method0.hh>
#include    <rtti/engine/helper/method1.hh>
#include    <rtti/engine/helper/method2.hh>
#include    <rtti/engine/helper/method3.hh>
#include    <rtti/engine/helper/method4.hh>
#include    <rtti/engine/helper/method5.hh>
#include    <rtti/engine/helper/method6.hh>
#include    <rtti/engine/helper/methodvararg.hh>

/*****************************************************************************/
#endif
