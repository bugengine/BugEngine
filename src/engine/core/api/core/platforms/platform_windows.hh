/* BugEngine / 2008-2012  Nicolas MERCIER <mercier.nicolas@gmail.com>
   see LICENSE for detail */

#ifndef BE_CORE_PLATFORMS_PLATFORM_WIN32_HH_
#define BE_CORE_PLATFORMS_PLATFORM_WIN32_HH_
/*****************************************************************************/

#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <windows.h>
#ifdef _MSC_VER
# pragma warning(push)
# pragma warning(disable:4505)
#endif
static inline void displayError()
{
    char* msg;
    ::FormatMessageA( FORMAT_MESSAGE_ALLOCATE_BUFFER|FORMAT_MESSAGE_FROM_SYSTEM,
                      0,
                      ::GetLastError(),
                      0,
                      (char*)&msg,
                      0,
                      0 );
    MessageBox(0, msg, "Win32 error", MB_OK);
    ::LocalFree(msg);
}

extern "C" inline int isatty(int /*fd*/)
{
    return 0;
}
#ifdef _MSC_VER
# pragma warning(pop)
#endif
#define BE_WIN32_PRINTERROR()       displayError()
#define BE_WIN32_CHECKRESULT(x)     if ((x) == (UINT)-1) BE_WIN32_PRINTERROR()


#ifdef _AMD64
# define BE_PLATFORM_NAME      Win64
# define BE_PLATFORM_WIN64     1
#else
# define BE_PLATFORM_NAME      Win32
#endif
#define BE_PLATFORM_WIN32      1
#define BE_PLATFORM_PC         1

#include    <stdlib.h>
#ifdef _MSC_VER
# define    malloca _alloca
# if _MSC_VER >= 1400
#  define   freea   _freea
# else
#  define   freea(p)    (void)p
# endif
#else
# include   <malloc.h>
# define    malloca alloca
# define    freea(p)	(void)p
#endif

/*****************************************************************************/
#endif
