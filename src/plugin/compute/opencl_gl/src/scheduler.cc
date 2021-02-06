/* BugEngine <bugengine.devel@gmail.com>
   see LICENSE for detail */

#include <stdafx.h>
#include <scheduler.hh>

namespace BugEngine { namespace KernelScheduler { namespace OpenCL_GL {

Scheduler::Scheduler(const Plugin::Context& context)
    : OpenCL::Scheduler(context, &createPlatformSpecificContextProperties()[0])
{
}

Scheduler::~Scheduler()
{
}

}}}  // namespace BugEngine::KernelScheduler::OpenCL_GL
