/* BugEngine / 2008-2012  Nicolas MERCIER <mercier.nicolas@gmail.com>
   see LICENSE for detail */

#ifndef BE_SAMPLES_KERNEL_KERNELTASK_SCRIPT_HH_
#define BE_SAMPLES_KERNEL_KERNELTASK_SCRIPT_HH_
/*****************************************************************************/
#include    <scheduler/kernel/kernel.script.hh>
#include    <scheduler/task/kerneltask.hh>
#include    <resource/resource.script.hh>
#include    <scheduler/kernel/stream.hh>

namespace BugEngine
{

class KernelSampleTask : public Kernel::Kernel
{
private:
    scoped<Task::KernelTask>                        m_kernelTask;
    scoped< BugEngine::Kernel::Stream<u32> >        m_out;
published:
    weak< BugEngine::Kernel::Stream<u32> > const    out;
published:
    KernelSampleTask(weak< BugEngine::Kernel::Stream<u32> > const in1, weak< BugEngine::Kernel::Stream<u32> > out1);
    ~KernelSampleTask();
};

}

/*****************************************************************************/
#endif
