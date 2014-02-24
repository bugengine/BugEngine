/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#ifndef BE_RTTI_ENGINE_HELPER_METHOD_HH_
#define BE_RTTI_ENGINE_HELPER_METHOD_HH_
/**************************************************************************************************/

namespace BugEngine { namespace RTTI
{

class Value;

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

template< typename T >
static Value createPod(Value* params, u32 paramCount)
{
    be_forceuse(params);
    be_forceuse(paramCount);
    be_assert(paramCount == 0, "too many parameters to POD construction");
    return Value(be_typeid<T>::type(), Value::Reserve);
}


template< typename T >
static Value createPodCopy(Value* params, u32 paramCount)
{
    be_forceuse(paramCount);
    be_assert(paramCount == 1, "invalid parameter count to POD copy");
    return params[0];
}

}}

/**************************************************************************************************/
#endif
