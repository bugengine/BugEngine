/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#ifndef BE_CORE_STDAFX_H_
#define BE_CORE_STDAFX_H_
/**************************************************************************************************/

#include    <kernel/stdafx.h>
#include    <kernel/simd.hh>

#ifndef BE_COMPUTE
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

#include    <core/platforms.hh>

#if defined(building_core)
# define    BE_API_CORE         BE_EXPORT
#elif defined(be_dll_core)
# define    BE_API_CORE         BE_IMPORT
#else
# define    BE_API_CORE
#endif


#include    <minitl/allocator.hh>

namespace BugEngine { namespace Arena
{
be_api(CORE) minitl::Allocator& temporary();
be_api(CORE) minitl::Allocator& stack();
be_api(CORE) minitl::Allocator& debug();
be_api(CORE) minitl::Allocator& general();
}}

#include    <minitl/assert.hh>

#include    <minitl/refcountable.hh>
#include    <minitl/rawptr.hh>
#include    <minitl/scopedptr.hh>
#include    <minitl/refptr.hh>
#include    <minitl/weakptr.hh>
#include    <minitl/cast.hh>

#include    <core/logger.hh>
#include    <core/string/istring.hh>
#include    <core/string/text.hh>

using minitl::raw;
using minitl::ref;
using minitl::weak;
using minitl::scoped;
using minitl::byref;
using minitl::be_checked_cast;
using minitl::be_const_cast;
using minitl::be_checked_numcast;
using minitl::be_function_cast;
#endif

/**************************************************************************************************/
#endif
