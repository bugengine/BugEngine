/* BugEngine <bugengine.devel@gmail.com>
   see LICENSE for detail */

#ifndef BE_NETWORK_STDAFX_H_
#define BE_NETWORK_STDAFX_H_
/**************************************************************************************************/

#include <bugengine/core/stdafx.h>

#if defined(building_network)
#    define BE_API_NETWORK BE_EXPORT
#elif defined(be_dll_network)
#    define BE_API_NETWORK BE_IMPORT
#else
#    define BE_API_NETWORK
#endif

/**************************************************************************************************/
#endif
