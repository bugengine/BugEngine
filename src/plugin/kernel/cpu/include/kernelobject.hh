/* BugEngine / 2008-2012  Nicolas MERCIER <mercier.nicolas@gmail.com>
   see LICENSE for detail */

#ifndef BE_KERNEL_CPU_KERNELOBJECT_HH_
#define BE_KERNEL_CPU_KERNELOBJECT_HH_
/*****************************************************************************/
#include    <plugin/dynobject.hh>
#include    <scheduler/task/task.hh>
#include    <scheduler/kernel/imemoryprovider.hh>

namespace BugEngine
{

class KernelObject;

struct CPUKernelTask
{
    weak<KernelObject>      object;
    weak<const Task::ITask> sourceTask;
    Kernel::KernelParameter params[16];
    i_u32                   splitCount;

    struct Range
    {
        u32     index;
        i_u32&  total;
        Range(i_u32& taskCount)
        :   index(taskCount)
        ,   total(taskCount)
        {
            taskCount++;
        }
        bool atomic() const { return false; }
        Range split()
        {
            return Range(total);
        }
    };

    CPUKernelTask(weak<KernelObject> object);
    Range prepare();
    void operator()(const Range& range) const;
};

class KernelObject : public minitl::refcountable
{
    friend class CPUKernelScheduler;
private:
    typedef Kernel::KernelParameter KernelParameterList[];
    typedef void(KernelMain)(const u32, const u32, const KernelParameterList params);
private:
    class Callback;
private:
    Plugin::DynamicObject               m_kernel;
    KernelMain*                         m_entryPoint;
    scoped< Task::Task<CPUKernelTask> > m_task;
    scoped< Task::ITask::ICallback >    m_callback;
    Task::ITask::CallbackConnection     m_callbackConnection;
public:
    KernelObject(const inamespace& name);
    ~KernelObject();

    void run(const u32 index, const u32 total, const KernelParameterList params);
};

}


/*****************************************************************************/
#endif
