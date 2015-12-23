/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#ifndef BE_KERNEL_CPU_KERNELOBJECT_HH_
#define BE_KERNEL_CPU_KERNELOBJECT_HH_
/**************************************************************************************************/
#include    <stdafx.h>
#include    <plugin/dynobject.hh>
#include    <scheduler/task/task.hh>
#include    <scheduler/kernel/imemoryprovider.hh>

namespace BugEngine
{

class KernelObject;

struct CPUKernelTask
{
    weak<KernelObject>      object;
    weak<Task::ITask>       sourceTask;
    Kernel::KernelParameter params[16];

    struct Range
    {
        u32 index;
        u32 total;
        Range(u32 total)
            :   index(total)
            ,   total(total)
        {
        }
        Range(u32 index, u32 total)
            :   index(index)
            ,   total(total)
        {
            be_assert(index != total, "index should not be equal to total");
        }
        bool atomic() const
        {
            return index != total;
        }
        u32 partCount(u32 workerCount) const
        {
            be_forceuse(workerCount);
            return total;
        }
        Range part(u32 index, u32 total) const
        {
            return Range(index, total);
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


/**************************************************************************************************/
#endif
