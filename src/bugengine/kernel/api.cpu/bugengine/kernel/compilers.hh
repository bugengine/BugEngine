/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#ifndef BE_KERNEL_CPU_COMPILERS_HH_
#define BE_KERNEL_CPU_COMPILERS_HH_
/**************************************************************************************************/

#if defined(__INTEL_COMPILER)
#    define BE_COMPILER_INTEL 1
#    define BE_COMPILER_NAME  "intel"
#    include <bugengine/kernel/compilers/intel.hh>
#elif defined(__clang__)
#    define BE_COMPILER_CLANG 1
#    define BE_COMPILER_NAME  "clang"
#    include <bugengine/kernel/compilers/clang.hh>
#elif defined(_MSC_VER)
#    define BE_COMPILER_MSVC 1
#    define BE_COMPILER_NAME "msvc"
#    include <bugengine/kernel/compilers/msvc.hh>
#elif defined(__GNUC__)
#    define BE_COMPILER_GCC  1
#    define BE_COMPILER_NAME "gcc"
#    include <bugengine/kernel/compilers/gcc.hh>
#elif defined(__SUNPRO_C) || defined(__SUNPRO_CC)
#    define BE_COMPILER_SUNCC 1
#    define BE_COMPILER_NAME  "suncc"
#    include <bugengine/kernel/compilers/suncc.hh>
#else
//# error unsupported compiler
#    include <bugengine/kernel/compilers/syntax.hh>
#endif

#ifdef __host
#    undef __host
#endif
#define __host
#ifdef __device
#    undef __device
#endif
#define __device inline
#ifdef __kernel
#    undef __kernel
#endif
#define __kernel inline

namespace BugEngine {

inline u32 bitCount(u32 bitMask)
{
    u32 result = 0;
    for(u32 i = 0; i < 32; ++i, bitMask >>= 1)
    {
        result += bitMask & 0x1;
    }
    return result;
}

inline u32 getFirstBit(u32 bitMask)
{
    for(u32 i = 0; i < 32; ++i, bitMask >>= 1)
    {
        if(bitMask & 0x1) return i;
    }
    return (u32)-1;
}

}  // namespace BugEngine

/**************************************************************************************************/
#endif
