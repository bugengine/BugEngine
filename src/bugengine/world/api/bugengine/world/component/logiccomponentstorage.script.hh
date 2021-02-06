/* BugEngine <bugengine.devel@gmail.com>
   see LICENSE for detail */

#ifndef BE_WORLD_COMPONENT_LOGICCOMPONENTSTORAGE_SCRIPT_HH_
#define BE_WORLD_COMPONENT_LOGICCOMPONENTSTORAGE_SCRIPT_HH_
/**************************************************************************************************/
#include <bugengine/world/stdafx.h>
#include <bugengine/world/component/icomponentstorage.script.hh>
#include <bugengine/world/component/storageconfiguration.script.hh>

#include <bugengine/scheduler/kernel/iproduct.script.hh>

namespace BugEngine { namespace World { namespace Component {

class LogicComponentStorage : public IComponentStorage
{
    published
        : LogicComponentStorage(weak< StorageConfiguration > configuration,
                                raw< const RTTI::Class >     component);

    /* TODO: dynamic type */
    const ref< KernelScheduler::IProduct > components;
};

}}}  // namespace BugEngine::World::Component

/**************************************************************************************************/
#endif
