/* BugEngine / 2008-2012  Nicolas MERCIER <mercier.nicolas@gmail.com>
   see LICENSE for detail */

#ifndef BE_KERNEL_COMPILERS_HH_
#define BE_KERNEL_COMPILERS_HH_
/*****************************************************************************/

#if defined(__INTEL_COMPILER)
# define BE_COMPILER_INTEL      1
# define BE_COMPILER_NAME       "intel"
# include <kernel/compilers/intel.hh>
#elif defined(__clang__)
# define BE_COMPILER_CLANG      1
# define BE_COMPILER_NAME       "clang"
# include <kernel/compilers/clang.hh>
#elif defined(_MSC_VER)
# define BE_COMPILER_MSVC       1
# define BE_COMPILER_NAME       "msvc"
# include <kernel/compilers/msvc.hh>
#elif defined(__GNUC__)
# define BE_COMPILER_GCC        1
# define BE_COMPILER_NAME       "gcc"
# include <kernel/compilers/gcc.hh>
#elif defined(__SUNPRO_CC)
# define BE_COMPILER_SUNCC      1
# define BE_COMPILER_NAME       "suncc"
# include <kernel/compilers/suncc.hh>
#else
# error unsupported compiler
# include <kernel/compilers/syntax.hh>
#endif

/*****************************************************************************/
#endif
