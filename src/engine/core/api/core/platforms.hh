/*****************************************************************************\
* BugEngine                                                                   *
* Copyright (C) 2005-2008  screetch <screetch@gmail.com>                      *
*                                                                             *
* This library is free software; you can redistribute it and/or modify it     *
* under the terms of the GNU Lesser General Public License as published by    *
* the Free Software Foundation; either version 2.1 of the License, or (at     *
* your option) any later version.                                             *
*                                                                             *
* This library is distributed in the hope that it will be useful, but WITHOUT *
* ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or       *
* FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public        *
* License for more details.                                                   *
*                                                                             *
* You should have received a copy of the GNU Lesser General Public License    *
* along with this library; if not, write to                                   *
* the Free Software Foundation, Inc.,                                         *
* 51 Franklin St,                                                             *
* Fifth Floor,                                                                *
* Boston, MA 02110-1301                                                       *
* USA                                                                         *
\*****************************************************************************/


#ifndef BE_CORE_PLATFORMS_H_
#define BE_CORE_PLATFORMS_H_
/*****************************************************************************/

#ifdef _WIN32
# define WIN32_LEAN_AND_MEAN
# define NOMINMAX
# include <windows.h>
static void displayError()
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
#define BE_WIN32_CHECKRESULT(x)     if((x) == -1) BE_WIN32_PRINTERROR()
#endif


#ifdef _WIN32
# ifdef _AMD64
#  define BE_PLATFORM_NAME  Win64
#  define BE_PLATFORM_WIN32 1
#  define BE_PLATFORM_WIN64 1
# else
#  define BE_PLATFORM_NAME  Win32
#  define BE_PLATFORM_WIN32 1
# endif
#elif defined __sun
# define BE_PLATFORM_NAME   SunOS
# define BE_PLATFORM_SUN    1
#else
# error "unknown platform"
#endif

/*****************************************************************************/
#endif
