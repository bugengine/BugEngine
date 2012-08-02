/* BugEngine / 2008-2012  Nicolas MERCIER <mercier.nicolas@gmail.com>
   see LICENSE for detail */

#include    <stdafx.h>
#include    <kerneltask.script.hh>
#include    <kernel/colors.hh>

namespace BugEngine { namespace Samples
{

KernelSampleTask::KernelSampleTask()
    :   Kernel("samples.kernel.add")
    ,   m_kernelTask(scoped<Task::KernelTask>::create(Arena::general(), "samples.kernel.add", Colors::Red::Red, Scheduler::High, this))
{
}

KernelSampleTask::~KernelSampleTask()
{
}

}}

