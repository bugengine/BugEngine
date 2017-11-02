/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#include    <stdafx.h>
#include    <scheduler.hh>


namespace BugEngine { namespace Kernel { namespace OpenCL_GL
{

Scheduler::Scheduler(const Plugin::Context& context)
    :   OpenCL::Scheduler(context, &createPlatformSpecificContextProperties()[0])
{
}

Scheduler::~Scheduler()
{
}

}}}
