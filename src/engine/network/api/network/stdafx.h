/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#ifndef BE_NETWORK_STDAFX_H_
#define BE_NETWORK_STDAFX_H_
/**************************************************************************************************/

#include <core/stdafx.h>

#if defined(building_network) || defined(NETWORK_EXPORTS)
# define    NETWORKEXPORT       BE_EXPORT
#else
# define    NETWORKEXPORT       BE_IMPORT
#endif


/**************************************************************************************************/
#endif
