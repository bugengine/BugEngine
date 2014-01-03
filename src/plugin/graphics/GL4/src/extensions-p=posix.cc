/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#include    <GL4/stdafx.h>
#include    <extensions.hh>
#include    <GL/glx.h>

namespace BugEngine { namespace OpenGL
{

Extension glGetExtension(const char *name)
{
    return glXGetProcAddress((const GLubyte*)name);
}

}}
