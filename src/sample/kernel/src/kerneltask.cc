/* BugEngine / 2008-2012  Nicolas MERCIER <mercier.nicolas@gmail.com>
   see LICENSE for detail */

#include    <stdafx.h>
#include    <kerneltask.script.hh>
#include    <scheduler/task/kerneltask.hh>
#include    <scheduler/kernel/product.hh>
#include    <kernel/colors.hh>


namespace BugEngine
{

KernelSampleTask::KernelSampleTask(const BugEngine::Kernel::Product< A >& in1,
                                   const BugEngine::Kernel::Product< B >& out1,
                                   const BugEngine::Kernel::Product< C >& out2,
                                   const BugEngine::Kernel::Product< D >& out3)
    :   KernelDescription("sample.kernel.add")
    ,   m_input1(in1)
    ,   m_input2(out1)
    ,   m_input3(out2)
    ,   m_input4(out3)
    ,   m_kernelTask(scoped<Task::KernelTask>::create(Arena::general(), "sample.kernel.add", Colors::Red::Red, Scheduler::High, this, makeParameters()))
    ,   m_chainInput1(in1.producer, m_kernelTask->startCallback())
    ,   m_chainInput2(out1.producer, m_kernelTask->startCallback())
    ,   m_chainInput3(out2.producer, m_kernelTask->startCallback())
    ,   m_chainInput4(out3.producer, m_kernelTask->startCallback())
    ,   output(out1.stream, m_kernelTask)
    ,   output2(out2.stream, m_kernelTask)
    ,   output3(out3.stream, m_kernelTask)
{
}

KernelSampleTask::~KernelSampleTask()
{
}

minitl::array< weak<const Kernel::IStream> > KernelSampleTask::makeParameters() const
{
    minitl::array< weak<const Kernel::IStream> > result(Arena::task(), 4);
    result[0] = m_input1.stream;
    result[1] = m_input2.stream;
    result[2] = m_input3.stream;
    result[3] = m_input4.stream;
    return result;
}

}

