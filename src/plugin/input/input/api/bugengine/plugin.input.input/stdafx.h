/* BugEngine <bugengine.devel@gmail.com>
   see LICENSE for detail */

#ifndef BE_INPUT_STDAFX_H_
#define BE_INPUT_STDAFX_H_
/**************************************************************************************************/

#include <bugengine/stdafx.h>

#if defined(building_input)
#    define BE_API_INPUT BE_EXPORT
#elif defined(be_dll_input)
#    define BE_API_INPUT BE_IMPORT
#else
#    define BE_API_INPUT
#endif

/**************************************************************************************************/
#endif
