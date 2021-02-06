/* BugEngine <bugengine.devel@gmail.com>
   see LICENSE for detail */

#ifndef BE_CONFIG_PLATFORMS_PLATFORM_FREEBSD_HH_
#define BE_CONFIG_PLATFORMS_PLATFORM_FREEBSD_HH_
/**************************************************************************************************/

#define BE_PLATFORM_NAME    FreeBSD
#define BE_PLATFORM_FREEBSD 1
#define BE_PLATFORM_BSD     1
#define BE_PLATFORM_POSIX   1
#define BE_PLATFORM_PC      1

#include <stdlib.h>
#define malloca  alloca
#define freea(p) (void)p
#define DIRENT_H <dirent.h>

/**************************************************************************************************/
#endif
