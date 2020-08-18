/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#ifndef BE_SCHEDULER_KERNEL_ICODELOADER_HH_
#define BE_SCHEDULER_KERNEL_ICODELOADER_HH_
/**************************************************************************************************/
#include <bugengine/scheduler/stdafx.h>
#include <bugengine/resource/loader.hh>

namespace BugEngine { namespace KernelScheduler {

class be_api(SCHEDULER) ICodeLoader : public Resource::ILoader
{
    BE_NOCOPY(ICodeLoader);

protected:
    ICodeLoader();
    ~ICodeLoader();
};

}}  // namespace BugEngine::KernelScheduler

/**************************************************************************************************/
#endif
