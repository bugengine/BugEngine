/* BugEngine / 2008-2012  Nicolas MERCIER <mercier.nicolas@gmail.com>
   see LICENSE for detail */

#ifndef BE_GL4_STDAFX_H_
#define BE_GL4_STDAFX_H_
/*****************************************************************************/

#include    <bugengine/stdafx.h>

#include    <3d/stdafx.h>
#include    <windowing/stdafx.h>
#include    <windowing/window.hh>
#include    <windowing/renderer.hh>

#if defined(building_GL4)
# define    GL4EXPORT      BE_EXPORT
#else
# define    GL4EXPORT      BE_IMPORT
#endif

#ifdef BE_PLATFORM_MACOS
# include   <OpenGL/OpenGL.h>
# include   <OpenGL/gl.h>
# ifndef APIENTRY
#  define APIENTRY
# endif
#else
# include   <GL/gl.h>
# include   <GL/glext.h>
#endif

/*****************************************************************************/
#endif
