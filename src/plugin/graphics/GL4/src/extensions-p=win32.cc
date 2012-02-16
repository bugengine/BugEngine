/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#include    <stdafx.h>
#include    <extensions.hh>

namespace BugEngine { namespace OpenGL
{

Extension glGetExtension(const char *name)
{
    return (Extension)wglGetProcAddress(name);
}

}}
