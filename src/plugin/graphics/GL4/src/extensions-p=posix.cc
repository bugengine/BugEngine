/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#include <bugengine/plugin.graphics.GL4/stdafx.h>
#include <GL/glx.h>
#include <extensions.hh>

namespace BugEngine { namespace OpenGL {

Extension glGetExtension(const char* name)
{
    return glXGetProcAddress((const GLubyte*)name);
}

}}  // namespace BugEngine::OpenGL
