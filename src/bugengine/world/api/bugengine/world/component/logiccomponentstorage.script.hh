/* BugEngine <bugengine.devel@gmail.com>
   see LICENSE for detail */

#ifndef BE_WORLD_COMPONENT_LOGICCOMPONENTSTORAGE_SCRIPT_HH_
#define BE_WORLD_COMPONENT_LOGICCOMPONENTSTORAGE_SCRIPT_HH_
/**************************************************************************************************/
#include <bugengine/world/stdafx.h>
#include <bugengine/world/component/icomponentstorage.script.hh>
#include <bugengine/world/component/storageconfiguration.script.hh>

#include <bugengine/rtti-ast/simplepolicy.factory.hh>
#include <bugengine/scheduler/kernel/iproduct.script.hh>
#include <bugengine/scheduler/kernel/parameters/iparameter.script.hh>

namespace BugEngine { namespace World { namespace Component {

class be_api(WORLD) LogicComponentStorage : public IComponentStorage
{
public:
    class IntrospectionHint : public RTTI::AST::IntrospectionHint
    {
    protected:
        virtual bool getPropertyType(RTTI::AST::DbContext& context, const istring name,
                                     RTTI::Type& propertyType) const override;

    public:
        IntrospectionHint(weak< const RTTI::AST::Object > owner, const RTTI::CallInfo& callInfo,
                          u32 argumentThis);
        ~IntrospectionHint();
    };

published:
    be_tag(RTTI::AST::SimplePolicy< LogicComponentStorage::IntrospectionHint >())
        LogicComponentStorage(weak< StorageConfiguration > configuration,
                              raw< const RTTI::Class >     componentType);

    const ref< KernelScheduler::IProduct > components;
};

}}}  // namespace BugEngine::World::Component

/**************************************************************************************************/
#endif
