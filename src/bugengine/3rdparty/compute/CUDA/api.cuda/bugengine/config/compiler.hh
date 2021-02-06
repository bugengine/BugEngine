/* BugEngine <bugengine.devel@gmail.com>
   see LICENSE for detail */

#ifndef BE_CONFIG_CUDA_COMPILER_HH_
#define BE_CONFIG_CUDA_COMPILER_HH_
/**************************************************************************************************/

#define __host    __host__
#define __device  __device__
#define __kernel  __device__

typedef signed char         i8;
typedef unsigned char       u8;
typedef signed short        i16;
typedef unsigned short      u16;
typedef signed int          i32;
typedef unsigned int        u32;
typedef signed long long    i64;
typedef unsigned long long  u64;
typedef u8                  byte;

#define be_break()
#define override
#define BE_NOINLINE
#define BE_ALWAYSINLINE
#define BE_SUPPORTS_EXCEPTIONS  0
#define BE_EXPORT
#define BE_IMPORT

#define be_alignof(t)           __alignof__(t)

#define kernel_private
#define kernel_local      __shared__
#define kernel_constant   __device const
#define kernel_global     __device
#define kernel_generic

/**************************************************************************************************/
#endif
