/* BugEngine / 2008-2012  Nicolas MERCIER <mercier.nicolas@gmail.com>
   see LICENSE for detail */

#ifndef BE_CORE_STDAFX_H_
#define BE_CORE_STDAFX_H_
/*****************************************************************************/

#include    <kernel/stdafx.h>
#include    <debug/stdafx.h>
#include    <minitl/stdafx.h>
#include    <core/preproc.hh>

#define     BE_FILE             __FILE__
#define     BE_LINE             __LINE__
#ifdef _MSC_VER
# define    BE_FUNCTION         __FUNCSIG__
#elif defined(__GNUC__)
# define    BE_FUNCTION         __PRETTY_FUNCTION__
#else
# define    BE_FUNCTION         __FUNCTION__
#endif
#define     BE_PROJECT          BE_STRINGIZE(BE_PROJECTCATEGORY) "." BE_STRINGIZE(BE_PROJECTNAME)
#define     BE_HERE             BE_FILE ":" BE_LINE "\n\t[ " BE_FUNCTION " ]\t"
#define     BE_NOCOPY(x)        private: x(const x&); x& operator=(const x&);

#include    <core/platforms.hh>

#if defined(building_core) || defined(CORE_EXPORTS)
# define    COREEXPORT          BE_EXPORT
#elif defined(core_dll)
# define    COREEXPORT          BE_IMPORT
#else
# define    COREEXPORT
#endif

#define     be_forceuse(p)   static_cast<void>(p)

template< typename T >
inline T be_align(T value, size_t alignment)
{
    size_t v = (size_t)(value);
    return (T)(alignment==v?v:((v+alignment-1) & ~(alignment-1)));
}

template< typename T >
inline T be_min(T t1, T t2)
{
    return t1 < t2 ? t1 : t2;
}

template< typename T >
inline T be_max(T t1, T t2)
{
    return t1 > t2 ? t1 : t2;
}

#include    <minitl/allocator.hh>

namespace BugEngine { namespace Arena
{
be_api(CORE) Allocator& temporary();
be_api(CORE) Allocator& stack();
be_api(CORE) Allocator& debug();
be_api(CORE) Allocator& general();
}}

#include    <debug/assert.hh>

#include    <minitl/refcountable.hh>
#include    <minitl/rawptr.hh>
#include    <minitl/scopedptr.hh>
#include    <minitl/refptr.hh>
#include    <minitl/weakptr.hh>
#include    <minitl/cast.hh>

#include    <core/logger.hh>

using minitl::raw;
using minitl::ref;
using minitl::weak;
using minitl::scoped;
using minitl::byref;
using minitl::be_checked_cast;
using minitl::be_const_cast;
using minitl::be_checked_numcast;


/*****************************************************************************/
#endif
