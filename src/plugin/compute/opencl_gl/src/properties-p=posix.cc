/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#include <stdafx.h>
#include <scheduler.hh>

namespace BugEngine { namespace KernelScheduler { namespace OpenCL_GL {

minitl::array< cl_context_properties > Scheduler::createPlatformSpecificContextProperties()
{
    be_info("not implemented yet.");
    minitl::array< cl_context_properties > properties(Arena::temporary(), 1);
    properties[0] = 0;
    return properties;
}

}}}  // namespace BugEngine::KernelScheduler::OpenCL_GL
