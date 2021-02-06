/* BugEngine <bugengine.devel@gmail.com>
   see LICENSE for detail */

#ifndef BE_CONFIG_PLATFORMS_PLATFORM_SUNOS_HH_
#define BE_CONFIG_PLATFORMS_PLATFORM_SUNOS_HH_
/**************************************************************************************************/

#define BE_PLATFORM_NAME  SunOS
#define BE_PLATFORM_SUN   1
#define BE_PLATFORM_POSIX 1
#define BE_PLATFORM_PC    1

#include <alloca.h>
#include <malloc.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define malloca  alloca
#define freea(p) (void)p
#define DIRENT_H <dirent.h>

/**************************************************************************************************/
#endif
