/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#ifndef BE_CONFIG_CL_COMPILERS_HH_
#define BE_CONFIG_CL_COMPILERS_HH_
/**************************************************************************************************/

#define __host
#define __device

typedef signed char    i8;
typedef unsigned char  u8;
typedef signed short   i16;
typedef unsigned short u16;
typedef signed int     i32;
typedef unsigned int   u32;
typedef signed long    i64;
typedef unsigned long  u64;
typedef u8             byte;

#define be_break()
#define override
#define BE_NOINLINE
#define BE_ALWAYSINLINE
#define BE_SUPPORTS_EXCEPTIONS 0
#define BE_EXPORT
#define BE_IMPORT

#define kernel_private  __private
#define kernel_local    __local
#define kernel_constant __constant
#define kernel_global   __global
#define kernel_generic  __generic

#define be_alignof(t) __alignof__(t)

#pragma OPENCL EXTENSION cl_khr_int64_base_atomics : enable
#include <opencl-c.h>

/**************************************************************************************************/
#endif
