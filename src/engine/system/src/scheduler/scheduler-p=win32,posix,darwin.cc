/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#include    <system/stdafx.h>
#include    <system/scheduler/scheduler.hh>
#include    <system/scheduler/kernel/ikernelscheduler.hh>
#include    <system/plugin.hh>

namespace BugEngine
{

void Scheduler::registerKernelSchedulers(const PluginContext& context)
{
    m_kernelSchedulers.push_back(Plugin<IKernelScheduler>("kernel.cpu", context));
}

}
