/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_CORE_COMPILERS_H_
#define BE_CORE_COMPILERS_H_
/*****************************************************************************/

#if defined(__INTEL_COMPILER)
# define BE_COMPILER_INTEL      1
# define BE_COMPILER_NAME       "intel"
# include <core/compilers/intel.hh>
#elif defined(_MSC_VER)
# define BE_COMPILER_MSVC       1
# define BE_COMPILER_NAME       "msvc"
# include <core/compilers/msvc.hh>
#elif defined(__GNUC__)
# define BE_COMPILER_GCC        1
# define BE_COMPILER_NAME       "gcc"
# include <core/compilers/gcc.hh>
#elif defined(__SUNPRO_CC)
# define BE_COMPILER_SUNCC      1
# define BE_COMPILER_NAME       "suncc"
# include <core/compilers/suncc.hh>
#elif defined(__ECLIPSE)
# define BE_COMPILER_NAME       "Eclipse syntax analyzer"
# include <core/compilers/syntax.hh>
#else
# error unsupported compiler
#endif

/*****************************************************************************/
#endif
