/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_CORE_PLATFORMS_WIN32_HH_
#define BE_CORE_PLATFORMS_WIN32_HH_
/*****************************************************************************/

#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <windows.h>
#pragma warning(disable:4505)
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
#define BE_WIN32_PRINTERROR()       displayError()
#define BE_WIN32_CHECKRESULT(x)     if((x) == (UINT)-1) BE_WIN32_PRINTERROR()


#ifdef _AMD64
# define BE_PLATFORM_NAME      Win64
# define BE_PLATFORM_WIN64     1
#else
# define BE_PLATFORM_NAME      Win32
#endif
#define BE_PLATFORM_WIN32      1
#define BE_PLATFORM_PC         1

/*****************************************************************************/
#endif
