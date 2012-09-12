/* BugEngine / 2008-2012  Nicolas MERCIER <mercier.nicolas@gmail.com>
   see LICENSE for detail */

#ifndef BE_MINITL_STDAFX_H_
#define BE_MINITL_STDAFX_H_
/*****************************************************************************/

#include    <kernel/stdafx.h>
#include    <minitl/features.hh>

#define     be_forceuse(p)   static_cast<void>(p)
#define     BE_NOCOPY(x)        private: x(const x&); x& operator=(const x&);
#define     be_api(module) module##EXPORT

#if defined(building_minitl) || defined(MINITL_EXPORTS)
# define    MINITLEXPORT         BE_EXPORT
#elif defined(minitl_dll)
# define    MINITLEXPORT         BE_IMPORT
#else
# define    MINITLEXPORT
#endif

namespace minitl
{

template< typename T >
inline T align(T value, size_t alignment)
{
    size_t v = (size_t)(value);
    return (T)(alignment==v?v:((v+alignment-1) & ~(alignment-1)));
}

template< typename T >
T min(T t1, T t2)
{
    return t1 < t2 ? t1 : t2;
}

template< typename T >
T max(T t1, T t2)
{
    return t1 > t2 ? t1 : t2;
}

template< typename T >
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
    return static_cast<T>(result + 1);
}

}

/*****************************************************************************/
#endif
