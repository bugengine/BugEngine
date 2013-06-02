/* BugEngine / 2008-2012  Nicolas MERCIER <mercier.nicolas@gmail.com>
   see LICENSE for detail */

#ifndef BE_CORE_PLATFORMS_PLATFORM_SUNOS_HH_
#define BE_CORE_PLATFORMS_PLATFORM_SUNOS_HH_
/*****************************************************************************/

#define BE_PLATFORM_NAME        SunOS
#define BE_PLATFORM_SUN         1
#define BE_PLATFORM_POSIX       1
#define BE_PLATFORM_PC          1

#include    <stdlib.h>
#include    <alloca.h>
#include    <malloc.h>
#include    <string.h>
#include    <stdio.h>
#define     malloca     alloca
#define     freea(p)    (void)p
#define     DIRENT_H    <dirent.h>

/*****************************************************************************/
#endif
