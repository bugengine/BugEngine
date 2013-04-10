/* BugEngine / 2008-2012  Nicolas MERCIER <mercier.nicolas@gmail.com>
   see LICENSE for detail */

#include    <GL4/stdafx.h>
#include    <extensions.hh>

namespace BugEngine { namespace OpenGL
{

Extension glGetExtension(const char *name)
{
    return (Extension)wglGetProcAddress(name);
}

}}
