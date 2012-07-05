/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#include    <stdafx.h>
#include    <clglkernelscheduler.hh>

namespace BugEngine
{

OpenCLOpenGLKernelScheduler::OpenCLOpenGLKernelScheduler(const PluginContext& context)
    :   OpenCLKernelScheduler(context, &createPlatformSpecificContextProperties()[0])
{
}

OpenCLOpenGLKernelScheduler::~OpenCLOpenGLKernelScheduler()
{
}

}
