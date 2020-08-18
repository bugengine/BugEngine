/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#ifndef BE_SCHEDULER_KERNEL_IPRODUCT_SCRIPT_HH_
#define BE_SCHEDULER_KERNEL_IPRODUCT_SCRIPT_HH_
/**************************************************************************************************/
#include <bugengine/scheduler/stdafx.h>
#include <bugengine/scheduler/kernel/parameters/iparameter.script.hh>
#include <bugengine/scheduler/task/itask.hh>

namespace BugEngine { namespace KernelScheduler {

class be_api(SCHEDULER) IProduct : public minitl::refcountable
{
protected:
    typedef minitl::tuple< weak< IMemoryHost >, u32 > HostInformation;
    ref< IParameter >                                 m_parameter;
    weak< Task::ITask >                               m_producer;
    minitl::vector< HostInformation >                 m_productOutput;

protected:
    IProduct(ref< IParameter > parameter, weak< Task::ITask > producer)
        : m_parameter(parameter)
        , m_producer(producer)
        , m_productOutput(Arena::task())
    {
    }

    ~IProduct()
    {
    }

public:
    weak< Task::ITask > producer() const
    {
        return m_producer;
    }

    void addOutputHost(weak< IMemoryHost > host);
    void removeOutputHost(weak< IMemoryHost > host);
};

}}  // namespace BugEngine::KernelScheduler

/**************************************************************************************************/
#endif
