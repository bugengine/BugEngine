/* BugEngine / 2008-2012  Nicolas MERCIER <mercier.nicolas@gmail.com>
   see LICENSE for detail */

#include    <stdafx.h>
#include    <clglkernelscheduler.hh>

namespace BugEngine
{

minitl::array<cl_context_properties> OpenCLOpenGLKernelScheduler::createPlatformSpecificContextProperties()
{
    CGLContextObj ctx = CGLGetCurrentContext();
    CGLShareGroupObj group = CGLGetShareGroup(ctx);
    if (group)
    {
        minitl::array<cl_context_properties> properties(Arena::temporary(), 3);
        properties[0] = CL_CONTEXT_PROPERTY_USE_CGL_SHAREGROUP_APPLE;
        properties[1] = (cl_context_properties)group;
        properties[2] = 0;
        return properties;
    }
    else
    {
        be_info("no OpenGL context found; OpenGL/OpenCL compatibility disabled");
        minitl::array<cl_context_properties> properties(Arena::temporary(), 1);
        properties[0] = 0;
        return properties;
    }
}

}

