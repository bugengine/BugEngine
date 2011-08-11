/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#include    <stdafx.h>
#include    <extensions.hh>
#include    <GL/glx.h>

namespace BugEngine { namespace Graphics { namespace OpenGL
{

Extension glGetExtension(const char *name)
{
    return glXGetProcAddress((const GLubyte*)name);
}

}}}
