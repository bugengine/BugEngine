/*****************************************************************************\
* BugEngine                                                                   *
* Copyright (C) 2005-2008  screetch <screetch@gmail.com>                      *
*                                                                             *
* This library is free software; you can redistribute it and/or modify it     *
* under the terms of the GNU Lesser General Public License as published by    *
* the Free Software Foundation; either version 2.1 of the License, or (at     *
* your option) any later version.                                             *
*                                                                             *
* This library is distributed in the hope that it will be useful, but WITHOUT *
* ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or       *
* FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public        *
* License for more details.                                                   *
*                                                                             *
* You should have received a copy of the GNU Lesser General Public License    *
* along with this library; if not, write to                                   *
* the Free Software Foundation, Inc.,                                         *
* 51 Franklin St,                                                             *
* Fifth Floor,                                                                *
* Boston, MA 02110-1301                                                       *
* USA                                                                         *
\*****************************************************************************/


#ifndef BE_CORE_STDAFX_H_
#define BE_CORE_STDAFX_H_
/*****************************************************************************/


#define     BE_STRINGIZE_IMPL(n)    #n
#define     BE_STRINGIZE(n)         BE_STRINGIZE_IMPL(n)
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
#define     FORCEEXPORT         BE_EXPORT

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



#define UNUSED(p)   static_cast<void>(p)

#include    <core/debug/assert.hh>
#include    <core/string/istring.hh>
#include    <core/log/logger.hh>
#include    <core/string/message.hh>

#include    <maths/float.hh>

#include    <minitl/ptr/refcountable.hh>
#include    <minitl/ptr/scopedptr.hh>
#include    <minitl/ptr/refptr.hh>
#include    <minitl/type/cast.hh>
#include    <core/endianness.hh>

using minitl::refptr;
using minitl::scopedptr;
using minitl::checked_cast;
using minitl::checked_numcast;


/*****************************************************************************/
#endif
