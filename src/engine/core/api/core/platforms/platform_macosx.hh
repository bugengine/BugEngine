/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#ifndef BE_CORE_PLATFORMS_PLATFORM_DARWIN_HH_
#define BE_CORE_PLATFORMS_PLATFORM_DARWIN_HH_
/*****************************************************************************/

#define BE_PLATFORM_NAME        MacOSX
#define BE_PLATFORM_MACOS       1
#define BE_PLATFORM_PC          1

#include <malloc/malloc.h>
#define malloca     alloca
#define freea(p)    (void)p

#define DIRENT_H    <sys/dir.h>
#define PLUGIN_EXT  ".dylib"

/*****************************************************************************/
#endif
