
/* BugEngine <bugengine.devel@gmail.com>
   see LICENSE for detail */

#include <bugengine/world/stdafx.h>
#include <bugengine/world/component/logiccomponentstorage.script.hh>

#include <bugengine/rtti/engine/objectinfo.script.hh>
#include <bugengine/scheduler/kernel/parameters/iparameter.script.hh>

namespace BugEngine { namespace World { namespace Component {

static RTTI::Value makeProduct(raw< const RTTI::Class >     componentType,
                               weak< StorageConfiguration > configuration)
{
    istring parameterName(minitl::format< 256u >("Segments<%s>") | componentType->name);
    raw< const RTTI::ObjectInfo > object
        = KernelScheduler::IParameter::getNamespace()->getStaticProperty(parameterName);
    if(object)
    {
        ref< KernelScheduler::IParameter > parameter
            = object->value(0, 0).as< ref< KernelScheduler::IParameter > >();
        return parameter->makeProduct(parameter, configuration->updateStart());
    }
    else
    {
        be_error("could not locate class %s" | parameterName);
        return RTTI::Value(ref< KernelScheduler::IProduct >());
    }
}

LogicComponentStorage::LogicComponentStorage(weak< StorageConfiguration > configuration,
                                             raw< const RTTI::Class >     componentType)
    : components(makeProduct(componentType, configuration))
{
    be_forceuse(configuration);
}

}}}  // namespace BugEngine::World::Component
