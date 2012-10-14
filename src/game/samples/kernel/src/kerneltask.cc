/* BugEngine / 2008-2012  Nicolas MERCIER <mercier.nicolas@gmail.com>
   see LICENSE for detail */

#include    <stdafx.h>
#include    <kerneltask.script.hh>
#include    <scheduler/task/kerneltask.hh>
#include    <scheduler/kernel/product.hh>
#include    <kernel/colors.hh>


namespace BugEngine
{

KernelSampleTask::KernelSampleTask(const BugEngine::Kernel::Product< u32 >& in1, const BugEngine::Kernel::Product< u32 >& out1)
    :   KernelDescription("samples.kernel.add")
    ,   m_kernelTask(scoped<Task::KernelTask>::create(Arena::general(), "samples.kernel.add", Colors::Red::Red, Scheduler::High, this))
    ,   m_input1(in1)
    ,   m_input2(out1)
    ,   m_chainInput1(in1.producer, m_kernelTask->startCallback())
    ,   m_chainInput2(out1.producer, m_kernelTask->startCallback())
    ,   output(out1.stream, m_kernelTask)
{
}

KernelSampleTask::~KernelSampleTask()
{
}

}

