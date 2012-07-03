/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#include    <stdafx.h>
#include    <clkernelscheduler.hh>

namespace BugEngine
{

void OpenCLKernelScheduler::fillPlatformSpecificContextProperties(const char* extensions, cl_context_properties* properties, int maxPropertyCount)
{
    if (strstr(extensions, "cl_APPLE_gl_sharing"))
    {
        CGLContextObj ctx = CGLGetCurrentContext();
        CGLShareGroupObj group = CGLGetShareGroup(ctx);
        if (group)
        {
            be_assert_recover(maxPropertyCount > 2, "no space to register OpenGL context; OpenGL/OpenCL compatibility disabled", return);
            properties[0] = CL_CONTEXT_PROPERTY_USE_CGL_SHAREGROUP_APPLE;
            properties[1] = (cl_context_properties)group;
        }
        else
        {
            be_info("no OpenGL context found; OpenGL/OpenCL compatibility disabled");
        }
    }
    else
    {
        be_info("no OpenGL sharing extension found; OpenGL/OpenCL compatibility disabled");
    }
}

}

