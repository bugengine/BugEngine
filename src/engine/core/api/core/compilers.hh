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
#else
# error unsupported compiler
#endif

/*****************************************************************************/
#endif
