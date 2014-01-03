/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#ifndef BE_CORE_PLATFORM_ANDROID_HH_
#define BE_CORE_PLATFORM_ANDROID_HH_
/*****************************************************************************/

#define BE_PLATFORM_NAME       Android
#define BE_PLATFORM_ANDROID    1
#define BE_PLATFORM_LINUX      1
#define BE_PLATFORM_POSIX      1

#include <malloc.h>
#define malloca     alloca
#define freea(p)    (void)p
#define DIRENT_H    <dirent.h>
#define PLUGIN_EXT  ".so"

/*****************************************************************************/
#endif
