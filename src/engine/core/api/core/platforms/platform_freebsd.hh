/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_CORE_PLATFORMS_FREEBSD_HH_
#define BE_CORE_PLATFORMS_FREEBSD_HH_
/*****************************************************************************/

#define BE_PLATFORM_NAME        BSD
#define BE_PLATFORM_BSD         1
#define BE_PLATFORM_POSIX       1
#define BE_PLATFORM_PC          1

#include <stdlib.h>
#define malloca alloca
#define freea(p)
#define DIRENT_H    <dirent.h>
#define PLUGIN_H    <system/posix/plugin.inl>

/*****************************************************************************/
#endif
