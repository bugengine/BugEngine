/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#ifndef BE_CORE_STDAFX_H_
#define BE_CORE_STDAFX_H_
/**************************************************************************************************/

#include <bugengine/kernel/stdafx.h>
#include <bugengine/kernel/simd.hh>

#ifndef BE_COMPUTE
#    include <bugengine/minitl/stdafx.h>

#    include <bugengine/core/preproc.hh>

#    define BE_FILE __FILE__
#    define BE_LINE __LINE__
#    ifdef _MSC_VER
#        define BE_FUNCTION __FUNCSIG__
#    elif defined(__GNUC__)
#        define BE_FUNCTION __PRETTY_FUNCTION__
#    else
#        define BE_FUNCTION __FUNCTION__
#    endif
#    define BE_PROJECT BE_STRINGIZE(BE_PROJECTCATEGORY) "." BE_STRINGIZE(BE_PROJECTNAME)
#    define BE_HERE    BE_FILE ":" BE_LINE "\n\t[ " BE_FUNCTION " ]\t"

#    include <bugengine/core/platforms.hh>

#    if defined(building_core)
#        define BE_API_CORE BE_EXPORT
#    elif defined(be_dll_core)
#        define BE_API_CORE BE_IMPORT
#    else
#        define BE_API_CORE
#    endif

#    include <bugengine/minitl/allocator.hh>

namespace BugEngine { namespace Arena {
be_api(CORE) minitl::Allocator& temporary();
be_api(CORE) minitl::Allocator& stack();
be_api(CORE) minitl::Allocator& debug();
be_api(CORE) minitl::Allocator& general();
}}  // namespace BugEngine::Arena

#    include <bugengine/minitl/assert.hh>

#    include <bugengine/minitl/cast.hh>
#    include <bugengine/minitl/rawptr.hh>
#    include <bugengine/minitl/refcountable.hh>
#    include <bugengine/minitl/refptr.hh>
#    include <bugengine/minitl/scopedptr.hh>
#    include <bugengine/minitl/weakptr.hh>

#    include <bugengine/core/logger.hh>
#    include <bugengine/core/string/istring.hh>
#    include <bugengine/core/string/text.hh>

using minitl::be_checked_cast;
using minitl::be_checked_numcast;
using minitl::be_const_cast;
using minitl::be_function_cast;
using minitl::byref;
using minitl::raw;
using minitl::ref;
using minitl::scoped;
using minitl::weak;
#endif

/**************************************************************************************************/
#endif
