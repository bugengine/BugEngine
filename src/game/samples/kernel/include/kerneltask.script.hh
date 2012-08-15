/* BugEngine / 2008-2012  Nicolas MERCIER <mercier.nicolas@gmail.com>
   see LICENSE for detail */

#ifndef BE_SAMPLES_KERNEL_KERNELTASK_SCRIPT_HH_
#define BE_SAMPLES_KERNEL_KERNELTASK_SCRIPT_HH_
/*****************************************************************************/
#include    <scheduler/kernel/kernel.script.hh>
#include    <scheduler/task/kerneltask.hh>
#include    <resource/resource.script.hh>
#include    <scheduler/kernel/stream.hh>

namespace BugEngine { namespace Samples
{

class KernelSampleTask : public Kernel::Kernel
{
private:
    scoped<Task::KernelTask>    m_kernelTask;
published:
    KernelSampleTask();
    ~KernelSampleTask();
};

}}

/*****************************************************************************/
#endif
