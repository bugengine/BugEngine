/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#include    <stdafx.h>
#include    <clglkernelscheduler.hh>

namespace BugEngine
{

minitl::array<cl_context_properties> OpenCLOpenGLKernelScheduler::createPlatformSpecificContextProperties()
{
    be_info("not implemented yet.");
    minitl::array<cl_context_properties> properties(Arena::temporary(), 1);
    properties[0] = 0;
    return properties;
}

}
