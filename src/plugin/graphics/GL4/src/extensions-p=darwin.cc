/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#include    <GL4/stdafx.h>
#include    <extensions.hh>
#include    <dlfcn.h>

namespace BugEngine { namespace OpenGL
{

Extension glGetExtension(const char *name)
{
    static void* image = dlopen("/System/Library/Frameworks/OpenGL.framework/Versions/Current/OpenGL", RTLD_LAZY);
    if (!image)
        return NULL;
    else
        return (Extension)dlsym (image, name);
}

}}
