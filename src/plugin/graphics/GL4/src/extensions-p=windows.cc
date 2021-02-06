/* BugEngine <bugengine.devel@gmail.com>
   see LICENSE for detail */

#include <bugengine/plugin.graphics.GL4/stdafx.h>
#include <extensions.hh>

namespace BugEngine { namespace OpenGL {

Extension glGetExtension(const char* name)
{
    return (Extension)wglGetProcAddress(name);
}

}}  // namespace BugEngine::OpenGL
