/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#ifndef BE_PYTHON_STDAFX_H_
#define BE_PYTHON_STDAFX_H_
/**************************************************************************************************/

#include    <bugengine/stdafx.h>

#if defined(building_python) || defined(PYTHON_EXPORTS)
# define     PYTHONEXPORT          BE_EXPORT
#elif defined(python_dll)
# define     PYTHONEXPORT          BE_IMPORT
#else
# define     PYTHONEXPORT
#endif

/**************************************************************************************************/
#endif
