/* BugEngine <bugengine.devel@gmail.com>
   see LICENSE for detail */

#include <stdafx.h>
#include <dlfcn.h>
#include <scheduler.hh>

namespace BugEngine { namespace KernelScheduler { namespace OpenCL_GL {

minitl::array< cl_context_properties > Scheduler::createPlatformSpecificContextProperties()
{
    CGLContextObj ctx = CGLGetCurrentContext();
    typedef CGLShareGroupObj (*t_CGLGetShareGroup)(CGLContextObj obj);
    t_CGLGetShareGroup b_CGLGetShareGroup
        = (t_CGLGetShareGroup)dlsym(RTLD_DEFAULT, "CGLGetShareGroup");
    if(!b_CGLGetShareGroup)
    {
        be_warning("CGLGetShareGroup not found; OpenGL/OpenCL compatibility disabled");
        minitl::array< cl_context_properties > properties(Arena::temporary(), 1);
        properties[0] = 0;
        return properties;
    }
    CGLShareGroupObj group = (*b_CGLGetShareGroup)(ctx);
    if(group)
    {
        minitl::array< cl_context_properties > properties(Arena::temporary(), 3);
        properties[0] = CL_CONTEXT_PROPERTY_USE_CGL_SHAREGROUP_APPLE;
        properties[1] = (cl_context_properties)group;
        properties[2] = 0;
        return properties;
    }
    be_info("no OpenGL context found; OpenGL/OpenCL compatibility disabled");
    minitl::array< cl_context_properties > properties(Arena::temporary(), 1);
    properties[0] = 0;
    return properties;
}

}}}  // namespace BugEngine::KernelScheduler::OpenCL_GL
