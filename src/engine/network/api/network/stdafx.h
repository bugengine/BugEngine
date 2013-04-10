/* BugEngine / 2008-2012  Nicolas MERCIER <mercier.nicolas@gmail.com>
   see LICENSE for detail */

#ifndef BE_NETWORK_STDAFX_H_
#define BE_NETWORK_STDAFX_H_
/*****************************************************************************/

#include <core/stdafx.h>

#if defined(building_network) || defined(NETWORK_EXPORTS)
# define    NETWORKEXPORT       BE_EXPORT
#else
# define    NETWORKEXPORT       BE_IMPORT
#endif


/*****************************************************************************/
#endif
