/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_OPENGL_STDAFX_H_
#define BE_OPENGL_STDAFX_H_
/*****************************************************************************/

#include    <core/stdafx.h>
#include    <rtti/stdafx.h>
#include    <system/stdafx.h>
#include    <graphics/stdafx.h>

#include    <windowing/stdafx.h>
#include    <windowing/window.hh>
#include    <windowing/renderer.hh>
#ifdef BE_PLATFORM_WIN32
# include   <GL/gl3.h>
# include   <GL/glext.h>
# include   <GL/wglext.h>
#else
# include   <GL/gl3.h>
# include   <GL/glext.h>
# include   <GL/glx.h>
# include   <GL/glxext.h>
#endif


/*****************************************************************************/
#endif
