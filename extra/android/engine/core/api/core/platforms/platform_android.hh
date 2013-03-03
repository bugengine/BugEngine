/* BugEngine / 2008-2012  Nicolas MERCIER <mercier.nicolas@gmail.com>
   see LICENSE for detail */

#ifndef BE_CORE_PLATFORM_ANDROID_HH_
#define BE_CORE_PLATFORM_ANDROID_HH_
/*****************************************************************************/

#define BE_PLATFORM_NAME       Android
#define BE_PLATFORM_ANDROID    1

#include <malloc.h>
#define malloca     alloca
#define freea(p)    (void)p
#define DIRENT_H    <dirent.h>
#define PLUGIN_EXT  ".so"

/*****************************************************************************/
#endif
