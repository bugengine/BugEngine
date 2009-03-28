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

#ifndef BE_RTTI_STDAFX_H_
#define BE_RTTI_STDAFX_H_
/*****************************************************************************/

#include    <core/stdafx.h>
#include    <core/log/logger.hh>
#include    <maths/vector.hh>
#include    <maths/matrix.hh>


#if defined(_WIN32)
# if defined(building_rtti) || defined(RTTI_EXPORTS)
#  define   RTTIEXPORT          __declspec(dllexport)
# elif defined(rtti_dll)
#  define   RTTIEXPORT          __declspec(dllimport)
# else
#  define    RTTIEXPORT
# endif
#else
# define    RTTIEXPORT
#endif

/*****************************************************************************/
#endif
