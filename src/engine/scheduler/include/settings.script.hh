/* BugEngine <bugengine.devel@gmail.com> under New BSD License
   see LICENSE for detail */

#ifndef BE_SCHEDULER_SETTINGS_SCRIPT_HH_
#define BE_SCHEDULER_SETTINGS_SCRIPT_HH_
/**************************************************************************************************/
#include <bugengine/scheduler/stdafx.h>
#include <bugengine/settings/settings.factory.hh>

namespace BugEngine { namespace SchedulerSettings {

struct Scheduler : public Settings::Settings< Scheduler >
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

}}  // namespace BugEngine::SchedulerSettings

/**************************************************************************************************/
#endif
