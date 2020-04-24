/* BugEngine <bugengine.devel@gmail.com> under New BSD License
   see LICENSE for detail */

#ifndef BE_SCHEDULER_SETTINGS_SCRIPT_HH_
#define BE_SCHEDULER_SETTINGS_SCRIPT_HH_
/**************************************************************************************************/
#include    <scheduler/stdafx.h>
#include    <settings/settings.factory.hh>


namespace BugEngine { namespace SchedulerSettings
{

struct Scheduler : public Settings::Settings<Scheduler>
{
    enum KernelSchedulerType
    {
        Auto,
        ForceCPU,
        ForceOpenCL,
        ForceCUDA,
        ForceVulkan
    };

    Scheduler();

    i32                 ThreadCount;
    KernelSchedulerType KernelScheduler;
};

}}

/**************************************************************************************************/
#endif
