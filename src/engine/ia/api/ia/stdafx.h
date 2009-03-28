/*****************************************************************************\
* VGE - Versatile Game Engine                                                 *
* Copyright (C) 2005-2006  screetch <screetch@gmail.com>                      *
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

#ifndef _VGEIA_STDAFX_H_
#define _VGEIA_STDAFX_H_
/*****************************************************************************/

#include <vge/core/stdafx.h>
#include <vge/rtti/stdafx.h>

#include <vge/rtti/helper.hh>


#if defined(WIN32)
# if defined(building_vgeia) || defined(IA_EXPORTS)
#  define   IAEXPORT            __declspec(dllexport)
# else  /* ! building_vgecore */
#  define   IAEXPORT            __declspec(dllimport)
# endif /* building_vgecore */
#else   /* ! WIN32, ! _DLL */
# define    IAEXPORT
#endif  /* MSVC */

/*****************************************************************************/
#endif
