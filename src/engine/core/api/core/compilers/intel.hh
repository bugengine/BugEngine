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


#ifndef BE_CORE_COMPILERS_H_
#define BE_CORE_COMPILERS_H_
/*****************************************************************************/

#define BE_ALIGNOF(t)          __alignof(t)
#define BE_SET_ALIGNMENT(n)    __declspec(align(n))
#define BE_ALIGN(size, align)   (align==size?size:(((size)+((align)-1)) & ~(align-1)))



typedef signed __int8          i8;
typedef signed __int16         i16;
typedef signed __int32         i32;
typedef signed __int64         i64;
typedef unsigned __int8        u8;
typedef unsigned __int16       u16;
typedef unsigned __int32       u32;
typedef unsigned __int64       u64;
typedef u8                      byte;

#ifdef _WIN32
# define _CRT_SECURE_NO_WARNINGS 1
# define _CRT_SECURE_NO_DEPRECATE 1
#endif
#define BE_THREAD_LOCAL        __declspec(thread)
#define BE_NOINLINE            __declspec(noinline)
#define BE_ALWAYSINLINE        __forceinline

#define NOTHROW     throw()


/*****************************************************************************/
#endif
