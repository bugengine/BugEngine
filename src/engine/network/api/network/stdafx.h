/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_NETWORK_STDAFX_H_
#define BE_NETWORK_STDAFX_H_
/*****************************************************************************/

#include <core/stdafx.h>

#if defined(building_network) || defined(NETWORK_EXPORTS)
# define    NETWORKEXPORT       BE_EXPORT
#elif defined(network_dll)
# define    NETWORKEXPORT       BE_IMPORT
#else
# define    NETWORKEXPORT
#endif


/*****************************************************************************/
#endif
