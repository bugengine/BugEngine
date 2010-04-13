/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_RENDERDX9_STDAFX_H_
#define BE_RENDERDX9_STDAFX_H_
/*****************************************************************************/

#include   <core/stdafx.h>
#include   <rtti/stdafx.h>
#include   <system/stdafx.h>
#include   <graphics/stdafx.h>
#include   <win32/stdafx.h>

#ifdef BE_COMPILER_MSVC
# pragma warning(disable:4985)
#endif

#include   <d3d9.h>
#include   <DxErr.h>
#include   <Cg/cg.h>

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
