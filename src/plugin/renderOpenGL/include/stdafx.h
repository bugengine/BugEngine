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


#ifdef BE_PLATFORM_MACOS
# define GL_ARB_vertex_shader    1
# define GL_ARB_fragment_shader  1
# define GL_EXT_geometry_shader4 1
# include   <OpenGL/OpenGL.h>
# include   <OpenGL/gl.h>
#else
# include   <GL/gl.h>
# include   <GL/glext.h>
#endif

/*****************************************************************************/
#endif
