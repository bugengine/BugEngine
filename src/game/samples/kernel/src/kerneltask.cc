/* BugEngine / 2008-2012  Nicolas MERCIER <mercier.nicolas@gmail.com>
   see LICENSE for detail */

#include    <stdafx.h>
#include    <kerneltask.script.hh>
#include    <kernel/colors.hh>

namespace BugEngine
{

KernelSampleTask::KernelSampleTask(weak< BugEngine::Kernel::Stream<u32> > in1, weak< BugEngine::Kernel::Stream<u32> > out1)
    :   Kernel("samples.kernel.add")
    ,   m_kernelTask(scoped<Task::KernelTask>::create(Arena::general(), "samples.kernel.add", Colors::Red::Red, Scheduler::High, this))
    ,   m_out(scoped< BugEngine::Kernel::Stream<u32> >::create(Arena::game()))
{
    be_forceuse(in1);
    be_forceuse(out1);
    be_info("created sample task!");
}

KernelSampleTask::~KernelSampleTask()
{
}

}

