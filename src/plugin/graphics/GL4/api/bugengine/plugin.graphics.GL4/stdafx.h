/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#ifndef BE_GL4_STDAFX_H_
#define BE_GL4_STDAFX_H_
/**************************************************************************************************/

#include <bugengine/stdafx.h>

#include <bugengine/plugin.graphics.3d/stdafx.h>
#include <bugengine/plugin.graphics.windowing/stdafx.h>
#include <bugengine/plugin.graphics.windowing/renderer.hh>
#include <bugengine/plugin.graphics.windowing/window.hh>

#if defined(building_GL4)
#    define BE_API_GL4 BE_EXPORT
#elif defined(be_dll_gl4)
#    define BE_API_GL4 BE_IMPORT
#else
#    define BE_API_GL4
#endif

#ifdef BE_PLATFORM_MACOS
#    include <OpenGL/OpenGL.h>
#    include <OpenGL/gl.h>
#    ifndef APIENTRY
#        define APIENTRY
#    endif
#else
#    define GL_GLEXT_PROTOTYPES 1
#    include <GL4/glcorearb.h>
#    include <GL4/glext.h>
#endif

/**************************************************************************************************/
#endif
