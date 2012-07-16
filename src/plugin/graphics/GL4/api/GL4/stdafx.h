/* BugEngine / 2008-2012  Nicolas MERCIER <mercier.nicolas@gmail.com>
   see LICENSE for detail */

#ifndef BE_GL4_STDAFX_H_
#define BE_GL4_STDAFX_H_
/*****************************************************************************/

#include    <core/stdafx.h>
#include    <rtti/stdafx.h>
#include    <system/stdafx.h>

#include    <3d/stdafx.h>
#include    <windowing/stdafx.h>
#include    <windowing/window.hh>
#include    <windowing/renderer.hh>


#ifdef BE_PLATFORM_MACOS
# define GL3_PROTOTYPES
# define GL_ARB_vertex_shader    1
# define GL_ARB_fragment_shader  1
# define GL_EXT_geometry_shader4 1
# include   <OpenGL/OpenGL.h>
# ifndef APIENTRY
#  define APIENTRY
# endif
#else
# include   <GL/gl.h>
# include   <GL/glext.h>
#endif

/*****************************************************************************/
#endif
