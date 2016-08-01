/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#ifndef BE_GLES2_STDAFX_H_
#define BE_GLES2_STDAFX_H_
/**************************************************************************************************/

#include    <bugengine/stdafx.h>

#include    <3d/stdafx.h>


#ifdef BE_PLATFORM_DARWIN
# include   <OpenGLES/ES2/gl.h>
# include   <OpenGLES/EAGL.h>
#else
# include   <GLES2/gl2.h>
# include   <EGL/egl.h>
#endif

/**************************************************************************************************/
#endif
