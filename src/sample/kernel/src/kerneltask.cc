/* BugEngine / 2008-2012  Nicolas MERCIER <mercier.nicolas@gmail.com>
   see LICENSE for detail */

#include    <stdafx.h>
#include    <kerneltask.script.hh>
#include    <scheduler/task/kerneltask.hh>
#include    <scheduler/kernel/product.hh>
#include    <kernel/colors.hh>


namespace BugEngine
{

KernelSampleTask::KernelSampleTask(const BugEngine::Kernel::Product< A >& in1, const BugEngine::Kernel::Product< B >& out1)
    :   KernelDescription("sample.kernel.add")
    ,   m_input1(in1)
    ,   m_input2(out1)
    ,   m_kernelTask(scoped<Task::KernelTask>::create(Arena::general(), "sample.kernel.add", Colors::Red::Red, Scheduler::High, this, makeParameters()))
    ,   m_chainInput1(in1.producer, m_kernelTask->startCallback())
    ,   m_chainInput2(out1.producer, m_kernelTask->startCallback())
    ,   output(out1.stream, m_kernelTask)
{
}

KernelSampleTask::~KernelSampleTask()
{
}

minitl::array< weak<const Kernel::IStream> > KernelSampleTask::makeParameters() const
{
    minitl::array< weak<const Kernel::IStream> > result(Arena::task(), 2);
    result[0] = m_input1.stream;
    result[1] = m_input2.stream;
    return result;
}

}

