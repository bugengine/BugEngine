/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#ifndef BE_PYTHON_STDAFX_H_
#define BE_PYTHON_STDAFX_H_
/**************************************************************************************************/

#include    <bugengine/stdafx.h>
#include    <pythonlib/stdafx.h>

#if defined(building_python)
# define     BE_API_PYTHON          BE_EXPORT
#elif defined(be_dll_python)
# define     BE_API_PYTHON          BE_IMPORT
#else
# define     BE_API_PYTHON
#endif


/**************************************************************************************************/
#endif
