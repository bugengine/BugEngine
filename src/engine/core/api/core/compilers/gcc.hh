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


#ifndef BE_CORE_COMPILERS_GCC_H_
#define BE_CORE_COMPILERS_GCC_H_
/*****************************************************************************/

#define BE_ALIGNOF(t)          __alignof__(t)
#define BE_ALIGN(size, align)   (align==size?size:(((size)+((align)-1)) & ~(align-1)))

#include <stdint.h>
#include <stdlib.h>
typedef int8_t                 i8;
typedef int16_t                i16;
typedef int32_t                i32;
typedef int64_t                i64;
typedef uint8_t                u8;
typedef uint16_t               u16;
typedef uint32_t               u32;
typedef uint64_t               u64;
typedef u8                      byte;

#define    override
#define BE_THREAD_LOCAL        __thread
#define BE_NOINLINE            __attribute__((noinline))
#define BE_ALWAYSINLINE        __attribute__((always_inline))

struct AlignmentHelper__
{
};
#define BE_SET_ALIGNMENT(n)     AlignmentHelper__ h__ __attribute__ ((aligned(n)))

/*****************************************************************************/
#endif
