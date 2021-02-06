/* BugEngine <bugengine.devel@gmail.com>
   see LICENSE for detail */

#include <bugengine/plugin.compute.cuda/stdafx.h>
#include <kernelobject.hh>

namespace BugEngine { namespace KernelScheduler { namespace Cuda {

CudaKernelTask::CudaKernelTask(weak< KernelObject > object)
    : object(object)
    , params(Arena::task(), 0)
{
}

CudaKernelTask::Range CudaKernelTask::prepare()
{
    return Range(1);
}

void CudaKernelTask::operator()(const Range& range) const
{
    object->run(range.index, range.total, params);
}

class KernelObject::Callback : public Task::ITask::ICallback
{
public:
    Callback()
    {
    }

private:
    virtual void onCompleted(weak< BugEngine::Scheduler > scheduler,
                             weak< Task::ITask >          task) override
    {
        be_checked_cast< Task::Task< CudaKernelTask > >(task)->body.sourceTask->completed(
            scheduler);
    }
    virtual void onConnected(weak< Task::ITask > /*to*/, CallbackStatus /*status*/) override
    {
    }
    virtual bool onDisconnected(weak< Task::ITask > /*from*/) override
    {
        return true;
    }
};

KernelObject::KernelObject(const inamespace& name)
    : m_kernel(name, ipath("kernel"))
    , m_entryPoint(m_kernel.getSymbol< KernelMain >("_kmain"))
    , m_task(scoped< Task::Task< CudaKernelTask > >::create(Arena::task(), istring(name.str().name),
                                                            Colors::make(231, 231, 231, 0),
                                                            CudaKernelTask(this)))
    , m_callback(scoped< Callback >::create(Arena::task()))
    , m_callbackConnection(m_task, m_callback)
{
    be_debug("kernel entry point: %p" | m_entryPoint);
}

KernelObject::~KernelObject()
{
}

void KernelObject::run(const u32 index, const u32 total,
                       const minitl::array< weak< const IMemoryBuffer > >& params)
{
    if(m_entryPoint) (*m_entryPoint)(index, total, params);
}

}}}  // namespace BugEngine::KernelScheduler::Cuda
