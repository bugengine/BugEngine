/* BugEngine / 2008-2012  Nicolas MERCIER <mercier.nicolas@gmail.com>
   see LICENSE for detail */

#ifndef BE_CORE_PLATFORMS_PLATFORM_LINUX_HH_
#define BE_CORE_PLATFORMS_PLATFORM_LINUX_HH_
/*****************************************************************************/

#define BE_PLATFORM_NAME        Linux
#define BE_PLATFORM_LINUX       1
#define BE_PLATFORM_POSIX       1
#define BE_PLATFORM_PC          1

#include    <strings.h>
#include    <stdlib.h>
#include    <malloc.h>
#define     stricmp strcasecmp
#define     strnicmp strncasecmp
#define     malloca alloca
#define     freea(p)
#define     DIRENT_H    <dirent.h>

/*****************************************************************************/
#endif
