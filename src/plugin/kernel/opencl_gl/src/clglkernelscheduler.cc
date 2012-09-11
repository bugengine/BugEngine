/* BugEngine / 2008-2012  Nicolas MERCIER <mercier.nicolas@gmail.com>
   see LICENSE for detail */

#include    <stdafx.h>
#include    <clglkernelscheduler.hh>

namespace BugEngine
{

OpenCLOpenGLKernelScheduler::OpenCLOpenGLKernelScheduler(const Plugin::Context& context)
    :   OpenCLKernelScheduler(context, &createPlatformSpecificContextProperties()[0])
{
}

OpenCLOpenGLKernelScheduler::~OpenCLOpenGLKernelScheduler()
{
}

}
