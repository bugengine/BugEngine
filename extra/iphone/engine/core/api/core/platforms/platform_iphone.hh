/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_CORE_PLATFORM_IPHONE_HH_
#define BE_CORE_PLATFORM_IPHONE_HH_
/*****************************************************************************/

#define BE_PLATFORM_NAME       iPhone
#define BE_PLATFORM_IPHONE     1
#define BE_PLATFORM_MACOS      1

#include <malloc/malloc.h>
#define malloca     alloca
#define freea(p)
#define DIRENT_H    <sys/dir.h>

/*****************************************************************************/
#endif
