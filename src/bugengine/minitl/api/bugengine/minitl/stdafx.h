/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#ifndef BE_MINITL_STDAFX_H_
#define BE_MINITL_STDAFX_H_
/**************************************************************************************************/

#include <bugengine/kernel/stdafx.h>

#if defined(BE_COMPUTE)
#    define be_api(module)
#else
#    define be_api(module) BE_API_##module
#endif

#if defined(building_minitl)
#    define BE_API_MINITL BE_EXPORT
#elif defined(be_dll_minitl)
#    define BE_API_MINITL BE_IMPORT
#else
#    define BE_API_MINITL
#endif

#include <bugengine/kernel/interlocked.hh>
#include <bugengine/kernel/simd.hh>
#include <bugengine/minitl/features.hh>

#define BE_NOCOPY(x)                                                                               \
private:                                                                                           \
    x(const x&);                                                                                   \
    x& operator=(const x&)

namespace minitl {

template < typename T >
inline T align(T value, size_t alignment)
{
    size_t v = (size_t)(value);
    return (T)(alignment == v ? v : ((v + alignment - 1) & ~(alignment - 1)));
}

template < typename T >
T min(T t1, T t2)
{
    return t1 < t2 ? t1 : t2;
}

template < typename T >
T max(T t1, T t2)
{
    return t1 > t2 ? t1 : t2;
}

template < typename T >
T nextPowerOf2(T t)
{
    u64 result = t;
    result -= 1;
    result |= result >> 1;
    result |= result >> 2;
    result |= result >> 4;
    result |= result >> 8;
    result |= result >> 16;
    result |= result >> 32;
    return static_cast< T >(result + 1);
}

}  // namespace minitl

/**************************************************************************************************/
#endif
