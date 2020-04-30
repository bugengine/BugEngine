/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#include <bugengine/scheduler/stdafx.h>
#include <bugengine/scheduler/kernel/ischeduler.hh>
#include <bugengine/scheduler/scheduler.hh>

namespace BugEngine { namespace KernelScheduler {

static minitl::vector< weak< IScheduler > > s_schedulers(Arena::task());

IScheduler::IScheduler(istring name, weak< Scheduler > scheduler, SchedulerType type)
    : m_scheduler(scheduler)
    , m_name(name)
    , m_type(type)
{
    s_schedulers.push_back(this);
}

IScheduler::~IScheduler()
{
    for(minitl::vector< weak< IScheduler > >::iterator it = s_schedulers.begin();
        it != s_schedulers.end(); ++it)
    {
        if(*it == this)
        {
            s_schedulers.erase(it);
            return;
        }
    }
    be_notreached();
}

weak< IScheduler > IScheduler::findScheduler(SchedulerType preferredType)
{
    weak< IScheduler > result;
    for(minitl::vector< weak< IScheduler > >::iterator it = s_schedulers.begin();
        it != s_schedulers.end(); ++it)
    {
        if((*it)->m_type == preferredType)
        {
            return *it;
        }
        else
        {
            result = *it;
        }
    }
    return result;
}

}}  // namespace BugEngine::KernelScheduler
