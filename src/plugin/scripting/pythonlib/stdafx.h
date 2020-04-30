/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#ifndef BE_PYTHON_STDAFX_H_
#define BE_PYTHON_STDAFX_H_
/**************************************************************************************************/

#include <bugengine/stdafx.h>

#if defined(building_python) || defined(building_py_bugengine)
#    define BE_API_PYTHON BE_EXPORT
#elif defined(python_dll)
#    define BE_API_PYTHON BE_IMPORT
#else
#    define BE_API_PYTHON
#endif

namespace BugEngine { namespace Arena {

be_api(PYTHON) minitl::Allocator& python();

}}  // namespace BugEngine::Arena

/**************************************************************************************************/
#endif
