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

#ifndef BE_GRAPHICS_STDAFX_H_
#define BE_GRAPHICS_STDAFX_H_
/*****************************************************************************/

#include <core/stdafx.h>
#include <rtti/stdafx.h>
#include <system/stdafx.h>
#include <input/stdafx.h>

#include <rtti/helper.hh>

#if defined(_WIN32)
# if defined(building_graphics) || defined(GRAPHICS_EXPORTS)
#  define   GRAPHICSEXPORT      __declspec(dllexport)
# elif defined(graphics_dll)
#  define   GRAPHICSEXPORT      __declspec(dllimport)
# else
#  define   GRAPHICSEXPORT
# endif
#else
# define    GRAPHICSEXPORT
#endif


/*****************************************************************************/
#endif
