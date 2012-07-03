/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#include    <stdafx.h>
#include    <clkernelscheduler.hh>

namespace BugEngine
{

void OpenCLKernelScheduler::fillPlatformSpecificContextProperties(cl_context_properties* properties, int maxPropertyCount)
{
    be_forceuse(properties);
    be_forceuse(maxPropertyCount);
    CGLContextObj ctx = CGLGetCurrentContext();
    be_info("%p" | ctx);
    CGLShareGroupObj group = CGLGetShareGroup(ctx);
    be_info("%p" | group);
}

}

