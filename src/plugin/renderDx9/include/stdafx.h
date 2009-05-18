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

#ifndef BE_RENDERDX9_STDAFX_H_
#define BE_RENDERDX9_STDAFX_H_
/*****************************************************************************/

#include    <core/stdafx.h>
#include    <rtti/stdafx.h>
#include    <system/stdafx.h>
#include    <graphics/stdafx.h>
#include    <win32/stdafx.h>

#include    <d3d9.h>
#include    <dxerr.h>
#include    <Cg/cg.h>

#define D3D_CHECKRESULT(operation)                          \
    {                                                       \
        HRESULT result_ = operation;                        \
        if(FAILED(result_))                                 \
        {                                                   \
            MessageBox( 0,                                  \
                        DXGetErrorDescription(result_),     \
                        DXGetErrorString(result_),          \
                        MB_OK);                             \
        }                                                   \
    }

/*****************************************************************************/
#endif
