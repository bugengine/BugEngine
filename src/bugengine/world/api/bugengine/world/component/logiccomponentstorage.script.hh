/* BugEngine <bugengine.devel@gmail.com>
   see LICENSE for detail */

#ifndef BE_WORLD_COMPONENT_LOGICCOMPONENTSTORAGE_SCRIPT_HH_
#define BE_WORLD_COMPONENT_LOGICCOMPONENTSTORAGE_SCRIPT_HH_
/**************************************************************************************************/
#include <bugengine/world/stdafx.h>
#include <bugengine/world/component/icomponentstorage.script.hh>
#include <bugengine/world/component/storageconfiguration.script.hh>

#include <bugengine/scheduler/kernel/iproduct.script.hh>
#include <bugengine/scheduler/kernel/parameters/iparameter.script.hh>

#include <bugengine/rtti/value.hh>

namespace BugEngine { namespace World { namespace Component {

class LogicComponentStorage : public IComponentStorage
{
    published
        :

        LogicComponentStorage(weak< StorageConfiguration > configuration,
                              raw< const RTTI::Class >     component);

    const RTTI::Value components;
};

}}}  // namespace BugEngine::World::Component

/**************************************************************************************************/
#endif
