/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#ifndef BE_WORLD_COMPONENT_STORAGECONFIGURATION_SCRIPT_HH_
#define BE_WORLD_COMPONENT_STORAGECONFIGURATION_SCRIPT_HH_
/**************************************************************************************************/
#include <bugengine/world/stdafx.h>
#include <bugengine/scheduler/task/itask.hh>

namespace BugEngine { namespace World { namespace Component {

class StorageConfiguration : public minitl::refcountable
{
private:
    ref< Task::ITask > m_updateStart;

private:
    void update();

public:
    weak< Task::ITask > updateStart() const
    {
        return m_updateStart;
    }

    published : StorageConfiguration();
    ~StorageConfiguration();
};

}}}  // namespace BugEngine::World::Component

/**************************************************************************************************/
#endif
