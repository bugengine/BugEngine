/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_CORE_STDAFX_H_
#define BE_CORE_STDAFX_H_
/*****************************************************************************/


#define     BE_STRINGIZE_IMPL(n)   #n
#define     BE_STRINGIZE(n)        BE_STRINGIZE_IMPL(n)
#define     BE_CONCAT_IMPL(i,j)    i##j
#define     BE_CONCAT(i,j)         BE_CONCAT_IMPL(i,j)
#ifndef BE_FILEID
# ifdef     _WIN32
#  define   BE_COUNTER          __COUNTER__
# else
#  define   BE_COUNTER          0
# endif
#endif
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

#include    <core/compilers.hh>
#include    <core/platforms.hh>
#include    <core/features.hh>

template< bool p >
struct StaticAssert_;

template< >
struct StaticAssert_<true>
{
    struct Defined
    {};
};

#define StaticAssert(expr) StaticAssert_<expr>::Defined ;

template< typename T >
T be_align(T value, size_t alignment)
{
    size_t v = (size_t)(value);
    return (T)(alignment==v?v:((v+alignment-1) & ~(alignment-1)));
}

#if defined(building_core) || defined(CORE_EXPORTS)
# define    COREEXPORT          BE_EXPORT
#elif defined(core_dll)
# define    COREEXPORT          BE_IMPORT
#else
# define    COREEXPORT
#endif
#define     NOEXPORT
#define     NONEEXPORT
#define     FORCEEXPORT         BE_EXPORT
#define     be_api(module) module##EXPORT

#include    <core/memory/malloc.hh>
#include    <cstdlib>
#include    <cstdio>
#include    <cstring>
#include    <string>
#include    <minitl/container/vector.hh>
#include    <minitl/container/list.hh>
#include    <minitl/container/stack.hh>
#include    <minitl/container/hash_map.hh>
#include    <minitl/container/hash_set.hh>
#include    <minitl/interlocked/interlocked.hh>
#include    <minitl/string/format.hh>



#define be_forceuse(p)   static_cast<void>(p)

#include    <core/debug/assert.hh>
#include    <core/string/istring.hh>
#include    <core/debug/logger.hh>
#include    <core/string/message.hh>

#include    <maths/float.hh>

#include    <minitl/ptr/refcountable.hh>
#include    <minitl/ptr/rawptr.hh>
#include    <minitl/ptr/scopedptr.hh>
#include    <minitl/ptr/refptr.hh>
#include    <minitl/ptr/weakptr.hh>
#include    <minitl/type/cast.hh>
#include    <core/endianness.hh>

using minitl::raw;
using minitl::ref;
using minitl::weak;
using minitl::scoped;
using minitl::be_checked_cast;
using minitl::be_const_cast;
using minitl::be_checked_numcast;


/*****************************************************************************/
#endif
