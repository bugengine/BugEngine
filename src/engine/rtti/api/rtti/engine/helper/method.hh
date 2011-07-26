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
    be_forceuse(src);
    ((T*)src)->~T();
}

template< size_t size >
inline void nullconstructor(const void* src, void* dst) { memcpy(dst, src, size); }
template< >
inline void nullconstructor<0>(const void* /*src*/, void* /*dst*/) {}
static inline void nulldestructor(void*) { }


template< typename T,
          typename P1=void,
          typename P2=void,
          typename P3=void,
          typename P4=void,
          typename P5=void,
          typename P6=void,
          typename P7=void,
          typename P8=void,
          typename P9=void,
          typename P10=void,
          typename P11=void,
          typename P12=void,
          typename P13=void,
          typename P14=void,
          typename P15=void,
          typename P16=void,
          typename Dummy=void>
struct procedurehelper;
template< typename T,
          typename R,
          typename P1=void,
          typename P2=void,
          typename P3=void,
          typename P4=void,
          typename P5=void,
          typename P6=void,
          typename P7=void,
          typename P8=void,
          typename P9=void,
          typename P10=void,
          typename P11=void,
          typename P12=void,
          typename P13=void,
          typename P14=void,
          typename P15=void,
          typename P16=void,
          typename Dummy=void>
struct functionhelper;


}}

#define BE_METHOD_PARAMS      0
#include    <rtti/engine/helper/methods.hh>
#undef BE_METHOD_PARAMS


#define BE_METHOD_PARAMS      1
#include    <rtti/engine/helper/methods.hh>
#undef BE_METHOD_PARAMS


#define BE_METHOD_PARAMS      2
#include    <rtti/engine/helper/methods.hh>
#undef BE_METHOD_PARAMS


#define BE_METHOD_PARAMS      3
#include    <rtti/engine/helper/methods.hh>
#undef BE_METHOD_PARAMS


#define BE_METHOD_PARAMS      4
#include    <rtti/engine/helper/methods.hh>
#undef BE_METHOD_PARAMS


#define BE_METHOD_PARAMS      5
#include    <rtti/engine/helper/methods.hh>
#undef BE_METHOD_PARAMS


#define BE_METHOD_PARAMS      6
#include    <rtti/engine/helper/methods.hh>
#undef BE_METHOD_PARAMS


#define BE_METHOD_PARAMS      7
#include    <rtti/engine/helper/methods.hh>
#undef BE_METHOD_PARAMS


#define BE_METHOD_PARAMS      8
#include    <rtti/engine/helper/methods.hh>
#undef BE_METHOD_PARAMS


#define BE_METHOD_PARAMS      9
#include    <rtti/engine/helper/methods.hh>
#undef BE_METHOD_PARAMS


#define BE_METHOD_PARAMS      10
#include    <rtti/engine/helper/methods.hh>
#undef BE_METHOD_PARAMS


#define BE_METHOD_PARAMS      11
#include    <rtti/engine/helper/methods.hh>
#undef BE_METHOD_PARAMS


#define BE_METHOD_PARAMS      12
#include    <rtti/engine/helper/methods.hh>
#undef BE_METHOD_PARAMS


#define BE_METHOD_PARAMS      13
#include    <rtti/engine/helper/methods.hh>
#undef BE_METHOD_PARAMS


#define BE_METHOD_PARAMS      14
#include    <rtti/engine/helper/methods.hh>
#undef BE_METHOD_PARAMS


#define BE_METHOD_PARAMS      15
#include    <rtti/engine/helper/methods.hh>
#undef BE_METHOD_PARAMS


#define BE_METHOD_PARAMS      16
#include    <rtti/engine/helper/methods.hh>
#undef BE_METHOD_PARAMS



#include    <rtti/engine/helper/methodvararg.hh>

/*****************************************************************************/
#endif
