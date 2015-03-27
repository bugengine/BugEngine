/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#ifndef BE_PYTHONLIB_STDAFX_H_
#define BE_PYTHONLIB_STDAFX_H_
/**************************************************************************************************/

#include    <bugengine/stdafx.h>

#if defined(building_pythonlib) || defined(PYTHONLIB_EXPORTS)
# define     PYTHONLIBEXPORT        BE_EXPORT
#elif defined(building_py_bugengine) || defined(PY_BUGENGINE_EXPORTS)
# define     PYTHONLIBEXPORT        BE_EXPORT
#elif defined(python_dll)
# define     PYTHONLIBEXPORT        BE_IMPORT
#else
# define     PYTHONLIBEXPORT
#endif

namespace BugEngine { namespace Arena
{

be_api(PYTHONLIB)
minitl::Allocator& python();

}}

/**************************************************************************************************/
#endif
