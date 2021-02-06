/* BugEngine <bugengine.devel@gmail.com>
   see LICENSE for detail */

#ifndef BE_CONFIG_PLATFORMS_PLATFORM_LINUX_HH_
#define BE_CONFIG_PLATFORMS_PLATFORM_LINUX_HH_
/**************************************************************************************************/

#define BE_PLATFORM_NAME  Linux
#define BE_PLATFORM_LINUX 1
#define BE_PLATFORM_POSIX 1
#define BE_PLATFORM_PC    1

#include <alloca.h>
#include <stdlib.h>
#include <strings.h>
#define stricmp  strcasecmp
#define strnicmp strncasecmp
#define malloca  alloca
#define freea(p) (void)p
#define DIRENT_H <dirent.h>

/**************************************************************************************************/
#endif
