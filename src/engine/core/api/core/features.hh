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


#ifndef BE_CORE_FEATURES_H_
#define BE_CORE_FEATURES_H_
/*****************************************************************************/

#define     BE_OPTIM_LEVEL_DEBUG    1
#define     BE_OPTIM_LEVEL_RELEASE  2
#define     BE_OPTIM_LEVEL_FINAL    3

#ifdef _DEBUG
# define    BE_DEBUG        1
# define    BE_FLAVOUR      Debug
# define    BE_OPTIM_LEVEL  BE_OPTIM_LEVEL_DEBUG
#elif !defined(NDEBUG)
# define    BE_RELEASE      1
# define    BE_FLAVOUR      Release
# define    BE_OPTIM_LEVEL  BE_OPTIM_LEVEL_RELEASE
#else
# define    BE_FINAL        1
# define    BE_FLAVOUR      Final
# define    BE_OPTIM_LEVEL  BE_OPTIM_LEVEL_FINAL
#endif

#define     BE_OPTIM_LEVEL_AT_MOST(x)       BE_OPTIM_LEVEL <= (x)
#define     BE_OPTIM_LEVEL_AT_LEAST(x)      BE_OPTIM_LEVEL >= (x)


#if BE_OPTIM_LEVEL_AT_MOST(BE_OPTIM_LEVEL_DEBUG)
# define BE_ENABLE_ASSERT               1
#endif

#if BE_OPTIM_LEVEL_AT_MOST(BE_OPTIM_LEVEL_RELEASE)
# define BE_ENABLE_LOGGING              1
# define BE_ENABLE_MEMORY_TRACKING      1
#endif

/*****************************************************************************/
#endif
