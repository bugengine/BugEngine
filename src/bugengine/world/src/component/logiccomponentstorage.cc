
/* BugEngine <bugengine.devel@gmail.com>
   see LICENSE for detail */

#include <bugengine/world/stdafx.h>
#include <bugengine/world/component/logiccomponentstorage.script.hh>

#include <bugengine/rtti-ast/node/object.hh>
#include <bugengine/rtti-ast/node/parameter.hh>
#include <bugengine/rtti/engine/objectinfo.script.hh>
#include <bugengine/scheduler/kernel/parameters/iparameter.script.hh>

namespace BugEngine { namespace World { namespace Component {

static RTTI::Type findProductType(raw< const RTTI::Class > componentType)
{
    istring parameterName(minitl::format< 256u >("Segments<%s>") | componentType->name);
    raw< const RTTI::Class > parameterClass
        = KernelScheduler::IParameter::getParameterClass(parameterName);
    if(parameterClass)
    {
        raw< const RTTI::ObjectInfo > productClass = parameterClass->getStaticProperty(
            KernelScheduler::IParameter::getProductTypePropertyName());
        if(productClass)
        {
            return productClass->value.as< const RTTI::Type& >();
        }
        else
        {
            be_error("class %s has not registered a %s" | parameterClass->name
                     | KernelScheduler::IParameter::getProductTypePropertyName());
            return be_type< ref< KernelScheduler::IProduct > >();
        }
    }
    else
    {
        be_error("class %s has not been registered as a parameter type" | parameterName);
        return be_type< ref< KernelScheduler::IProduct > >();
    }
}

struct Visitor : public RTTI::AST::Node::Visitor
{
    RTTI::Type result;

    Visitor() : result(be_type< ref< KernelScheduler::IProduct > >())
    {
    }

    using RTTI::AST::Node::Visitor::accept;
    void accept(weak< const RTTI::AST::Parameter > parameter, istring name,
                weak< const RTTI::AST::Node > value)
    {
        be_forceuse(parameter);
        be_forceuse(name);
        value->visit(*this);
    }
    void accept(weak< const RTTI::AST::Reference > reference, const RTTI::Value& referencedValue)
    {
        be_forceuse(reference);
        result = findProductType(referencedValue.as< raw< const RTTI::Class > >());
    }
};

LogicComponentStorage::IntrospectionHint::IntrospectionHint(weak< const RTTI::AST::Object > owner,
                                                            const RTTI::CallInfo& callInfo,
                                                            u32                   argumentThis)
    : RTTI::AST::IntrospectionHint(owner, callInfo, argumentThis)
{
}

LogicComponentStorage::IntrospectionHint::~IntrospectionHint()
{
}

bool LogicComponentStorage::IntrospectionHint::getPropertyType(RTTI::AST::DbContext& context,
                                                               const istring         name,
                                                               RTTI::Type& propertyType) const
{
    /* overrides type of "components" property */
    bool result = RTTI::AST::IntrospectionHint::getPropertyType(context, name, propertyType);
    if(result && name == istring("components"))
    {
        weak< const RTTI::AST::Parameter > parameter = m_owner->getParameter("componentType");
        be_assert(parameter, "could not locate parameter %s" | name);
        Visitor visitor;
        parameter->visit(visitor);
        propertyType = visitor.result;
    }
    return result;
}

static ref< KernelScheduler::IProduct > makeProduct(raw< const RTTI::Class >     componentType,
                                                    weak< StorageConfiguration > configuration)
{
    istring parameterName(minitl::format< 256u >("Segments<%s>") | componentType->name);
    raw< const RTTI::Class > parameterClass
        = KernelScheduler::IParameter::getParameterClass(parameterName);
    if(parameterClass)
    {
        ref< KernelScheduler::IParameter > parameter
            = parameterClass->constructor->doCall(0, 0).as< ref< KernelScheduler::IParameter > >();
        return parameter->makeProduct(parameter, configuration->updateStart());
    }
    else
    {
        be_error("could not locate class %s" | parameterName);
        return ref< KernelScheduler::IProduct >();
    }
}

LogicComponentStorage::LogicComponentStorage(weak< StorageConfiguration > configuration,
                                             raw< const RTTI::Class >     componentType)
    : components(makeProduct(componentType, configuration))
{
    be_forceuse(configuration);
}

}}}  // namespace BugEngine::World::Component
