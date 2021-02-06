
/* BugEngine <bugengine.devel@gmail.com>
   see LICENSE for detail */

#include <bugengine/world/stdafx.h>
#include <bugengine/world/component/logiccomponentstorage.script.hh>

#include <bugengine/rtti/engine/objectinfo.script.hh>
#include <bugengine/scheduler/kernel/parameters/iparameter.script.hh>

namespace BugEngine { namespace World { namespace Component {

LogicComponentStorage::LogicComponentStorage(weak< StorageConfiguration > configuration,
                                             raw< const RTTI::Class >     componentType)
    : components()
{
    be_forceuse(configuration);
    be_forceuse(componentType);

    istring parameterName(minitl::format< 256u >("Segments<%s>") | componentType->name);
    raw< const RTTI::ObjectInfo > object = KernelScheduler::IParameter::getNamespace()->objects;
    while(object)
    {
        if(object->name == parameterName)
        {
            be_info("found");
        }
        object = object->next;
    }
}

}}}  // namespace BugEngine::World::Component
