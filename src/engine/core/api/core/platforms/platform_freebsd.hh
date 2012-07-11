/* BugEngine / 2008-2012  Nicolas MERCIER <mercier.nicolas@gmail.com>
   see LICENSE for detail */

#ifndef BE_CORE_PLATFORMS_PLATFORM_FREEBSD_HH_
#define BE_CORE_PLATFORMS_PLATFORM_FREEBSD_HH_
/*****************************************************************************/

#define BE_PLATFORM_NAME        BSD
#define BE_PLATFORM_BSD         1
#define BE_PLATFORM_POSIX       1
#define BE_PLATFORM_PC          1

#include <stdlib.h>
#define malloca alloca
#define freea(p)
#define DIRENT_H    <dirent.h>
#define PLUGIN_H    <system/plugin/posix/plugin.inl>

/*****************************************************************************/
#endif
