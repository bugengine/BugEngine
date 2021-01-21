/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#ifndef BE_CONFIG_COMPILER_HH_
#define BE_CONFIG_COMPILER_HH_
/**************************************************************************************************/

#if defined(__INTEL_COMPILER)
#    define BE_COMPILER_INTEL 1
#    define BE_COMPILER_NAME  "intel"
#    include <bugengine/config/compilers/intel.hh>
#elif defined(__clang__)
#    define BE_COMPILER_CLANG 1
#    define BE_COMPILER_NAME  "clang"
#    include <bugengine/config/compilers/clang.hh>
#elif defined(_MSC_VER)
#    define BE_COMPILER_MSVC 1
#    define BE_COMPILER_NAME "msvc"
#    include <bugengine/config/compilers/msvc.hh>
#elif defined(__GNUC__)
#    define BE_COMPILER_GCC  1
#    define BE_COMPILER_NAME "gcc"
#    include <bugengine/config/compilers/gcc.hh>
#elif defined(__SUNPRO_C) || defined(__SUNPRO_CC)
#    define BE_COMPILER_SUNCC 1
#    define BE_COMPILER_NAME  "suncc"
#    include <bugengine/config/compilers/suncc.hh>
#else
//# error unsupported compiler
#    include <bugengine/config/compilers/syntax.hh>
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

#define kernel_constant const
#define kernel_global
#define kernel_local
#define kernel_private
#define kernel_generic

/**************************************************************************************************/
#endif
