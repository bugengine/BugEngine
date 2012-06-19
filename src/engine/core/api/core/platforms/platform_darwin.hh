/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_CORE_PLATFORMS_DARWIN_HH_
#define BE_CORE_PLATFORMS_DARWIN_HH_
/*****************************************************************************/

#define BE_PLATFORM_NAME        MacOSX
#define BE_PLATFORM_MACOS       1
#define BE_PLATFORM_PC          1

#include <malloc/malloc.h>
#define malloca     alloca
#define freea(p)

#define DIRENT_H    <sys/dir.h>
#define PLUGIN_EXT  ".dylib"
#define PLUGIN_H    <system/plugin/posix/plugin.inl>

/*****************************************************************************/
#endif
