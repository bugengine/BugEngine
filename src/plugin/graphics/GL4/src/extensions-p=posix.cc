/* BugEngine / 2008-2012  Nicolas MERCIER <mercier.nicolas@gmail.com>
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
