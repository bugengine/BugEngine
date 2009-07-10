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

#if BE_PLATFORM_WIN32 == 1
# define    FORCEEXPORT         __declspec(dllexport)
# if defined(building_core) || defined(CORE_EXPORTS)
#  define   COREEXPORT          __declspec(dllexport)
# elif defined(core_dll)
#  define   COREEXPORT          __declspec(dllimport)
#  error
# else
#  define   COREEXPORT
# endif
#else
# define    FORCEEXPORT
# define    COREEXPORT
#endif

#include    <core/memory/malloc.hh>
#include    <cstdlib>
#include    <cstdio>
#include    <cstring>
#include    <string>
#include    <minitl/container/vector>
#include    <minitl/container/list>
#include    <minitl/container/stack>
#include    <minitl/container/hash_map>
#include    <minitl/container/hash_set>
#include    <minitl/interlocked/interlocked>
#include    <minitl/string/format.hh>



#define UNUSED(p)   static_cast<void>(p)

#include    <core/debug/assert.hh>
#include    <core/string/istring.hh>
#include    <core/log/logger.hh>
#include    <core/string/message.hh>

#include    <maths/float.hh>

#include    <minitl/ptr/refcountable>
#include    <minitl/ptr/scopedptr>
#include    <minitl/ptr/refptr>
#include    <minitl/type/cast>

using minitl::refptr;
using minitl::scopedptr;
using minitl::checked_cast;
using minitl::checked_numcast;


/*****************************************************************************/
#endif
