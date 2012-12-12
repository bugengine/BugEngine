/* BugEngine / 2008-2012  Nicolas MERCIER <mercier.nicolas@gmail.com>
   see LICENSE for detail */

#ifndef BE_SAMPLES_KERNEL_KERNELTASK_SCRIPT_HH_
#define BE_SAMPLES_KERNEL_KERNELTASK_SCRIPT_HH_
/*****************************************************************************/
#include    <scheduler/kernel/kernel.script.hh>
#include    <scheduler/task/itask.hh>
#include    <scheduler/kernel/product.hh>

namespace BugEngine
{

class KernelSampleTask : public Kernel::KernelDescription
{
private:
    BugEngine::Kernel::Product< u32 > const m_input1;
    BugEngine::Kernel::Product< u32 > const m_input2;
    scoped< BugEngine::Task::ITask > const  m_kernelTask;
    Task::ITask::CallbackConnection const   m_chainInput1;
    Task::ITask::CallbackConnection const   m_chainInput2;
published:
    BugEngine::Kernel::Product< u32 > const output;
published:
    KernelSampleTask(const BugEngine::Kernel::Product< u32 >& in1, const BugEngine::Kernel::Product< u32 >& out1);
    ~KernelSampleTask();
private:
    minitl::array< weak<const Kernel::IStream> > makeParameters() const;
};

}

/*****************************************************************************/
#endif
