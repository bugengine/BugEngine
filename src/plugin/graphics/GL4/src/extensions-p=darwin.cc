/* BugEngine <bugengine.devel@gmail.com>
   see LICENSE for detail */

#include <bugengine/plugin.graphics.GL4/stdafx.h>
#include <dlfcn.h>
#include <extensions.hh>

namespace BugEngine { namespace OpenGL {

Extension glGetExtension(const char* name)
{
    static void* image
        = dlopen("/System/Library/Frameworks/OpenGL.framework/Versions/Current/OpenGL", RTLD_LAZY);
    if(!image)
        return NULL;
    else
        return (Extension)dlsym(image, name);
}

}}  // namespace BugEngine::OpenGL
