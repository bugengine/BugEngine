/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_CORE_PLATFORMS_LINUX_HH_
#define BE_CORE_PLATFORMS_LINUX_HH_
/*****************************************************************************/

#define BE_PLATFORM_NAME        Linux
#define BE_PLATFORM_LINUX       1
#define BE_PLATFORM_POSIX       1
#define BE_PLATFORM_PC          1
#include   <strings.h>
#define    stricmp strcasecmp
#define    strnicmp strncasecmp

/*****************************************************************************/
#endif
