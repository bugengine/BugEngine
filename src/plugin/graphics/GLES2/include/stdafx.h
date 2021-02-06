/* BugEngine <bugengine.devel@gmail.com>
   see LICENSE for detail */

#ifndef BE_GLES2_STDAFX_H_
#define BE_GLES2_STDAFX_H_
/**************************************************************************************************/

#include <bugengine/stdafx.h>

#include <bugengine/plugin.graphics.3d/stdafx.h>

#ifdef BE_PLATFORM_DARWIN
#    include <OpenGLES/EAGL.h>
#    include <OpenGLES/ES2/gl.h>
#else
#    include <EGL/egl.h>
#    include <GLES2/gl2.h>
#endif

/**************************************************************************************************/
#endif
