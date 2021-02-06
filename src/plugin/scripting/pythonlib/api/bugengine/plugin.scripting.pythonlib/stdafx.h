/* BugEngine <bugengine.devel@gmail.com>
   see LICENSE for detail */

#ifndef BE_PYTHONLIB_STDAFX_H_
#define BE_PYTHONLIB_STDAFX_H_
/**************************************************************************************************/

#include <bugengine/stdafx.h>

#if defined(building_pythonlib)
#    define BE_API_PYTHONLIB BE_EXPORT
#elif defined(be_dll_pythonlib)
#    define BE_API_PYTHONLIB BE_IMPORT
#else
#    define BE_API_PYTHONLIB
#endif

namespace BugEngine { namespace Arena {

be_api(PYTHONLIB) minitl::Allocator& python();

}}  // namespace BugEngine::Arena

/**************************************************************************************************/
#endif
