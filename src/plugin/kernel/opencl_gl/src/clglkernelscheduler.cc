/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
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
