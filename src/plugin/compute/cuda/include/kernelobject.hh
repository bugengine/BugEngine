/* BugEngine <bugengine.devel@gmail.com>
   see LICENSE for detail */

#ifndef BE_COMPUTE_CUDA_KERNELOBJECT_HH_
#define BE_COMPUTE_CUDA_KERNELOBJECT_HH_
/**************************************************************************************************/
#include <bugengine/plugin.compute.cuda/stdafx.h>
#include <bugengine/plugin/dynobject.hh>
#include <bugengine/scheduler/task/task.hh>

namespace BugEngine { namespace KernelScheduler { namespace Cuda {

class KernelObject;
class Scheduler;

struct CudaKernelTask
{
    weak< KernelObject >                         object;
    weak< Task::ITask >                          sourceTask;
    minitl::array< weak< const IMemoryBuffer > > params;

    struct Range
    {
        u32 index;
        u32 total;
        Range(u32 total) : index(total), total(total)
        {
        }
        Range(u32 index, u32 total) : index(index), total(total)
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
        Range part(u32 i, u32 t) const
        {
            return Range(i, t);
        }
    };

    CudaKernelTask(weak< KernelObject > object);
    Range prepare();
    void  operator()(const Range& range) const;
};

class KernelObject : public minitl::refcountable
{
    friend class Scheduler;

private:
    typedef void(KernelMain)(const u32, const u32,
                             const minitl::array< weak< const IMemoryBuffer > >& params);

private:
    class Callback;

private:
    Plugin::DynamicObject                  m_kernel;
    KernelMain*                            m_entryPoint;
    scoped< Task::Task< CudaKernelTask > > m_task;
    scoped< Task::ITask::ICallback >       m_callback;
    Task::ITask::CallbackConnection        m_callbackConnection;

public:
    KernelObject(const inamespace& name);
    ~KernelObject();

    void run(const u32 index, const u32 total,
             const minitl::array< weak< const IMemoryBuffer > >& params);
};

}}}  // namespace BugEngine::KernelScheduler::Cuda

/**************************************************************************************************/
#endif
