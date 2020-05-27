/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#ifndef BE_SCHEDULER_KERNEL_CODE_SCRIPT_HH_
#define BE_SCHEDULER_KERNEL_CODE_SCRIPT_HH_
/**************************************************************************************************/
#include <bugengine/scheduler/stdafx.h>
#include <bugengine/resource/description.script.hh>

namespace BugEngine { namespace KernelScheduler {

class be_api(SCHEDULER) Code : public Resource::Description
{
protected:
    const inamespace m_name;

public:
    Code(const inamespace& name);
    ~Code();

    inamespace name() const
    {
        return m_name;
    }
};

}}  // namespace BugEngine::KernelScheduler

/**************************************************************************************************/
#endif
