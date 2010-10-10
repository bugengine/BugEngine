/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_OPENGL_STDAFX_H_
#define BE_OPENGL_STDAFX_H_
/*****************************************************************************/

#include    <core/stdafx.h>
#include    <rtti/stdafx.h>
#include    <system/stdafx.h>
#include    <graphics/stdafx.h>

#ifdef BE_PLATFORM_WIN32
# include   <win32/stdafx.h>
# include   <win32/window.hh>
# include   <win32/renderer.hh>
# include   <GL/gl3.h>
# include   <GL/glext.h>
# include   <GL/wglext.h>
namespace BugEngine { namespace Graphics
{
namespace Windowing = Win32;
}}
#else
# include   <X/stdafx.h>
# include   <X/window.hh>
# include   <X/renderer.hh>
# include   <GL/glext.h>
# include   <GL/glx.h>
# include   <GL/glxext.h>
namespace BugEngine { namespace Graphics
{
namespace Windowing = X;
}}
#endif


/*****************************************************************************/
#endif
