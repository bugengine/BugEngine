/* BugEngine <bugengine.devel@gmail.com>
   see LICENSE for detail */

#include <bugengine/rtti-ast/stdafx.h>
#include <bugengine/rtti-ast/introspectionhint.hh>

#include <bugengine/rtti-ast/dbcontext.hh>
#include <bugengine/rtti-ast/node/node.hh>
#include <bugengine/rtti-ast/node/object.hh>
#include <bugengine/rtti/engine/objectinfo.script.hh>
#include <bugengine/rtti/engine/propertyinfo.script.hh>

namespace BugEngine { namespace RTTI { namespace AST {

IntrospectionHint::IntrospectionHint(weak< const Object > owner, const CallInfo& callInfo,
                                     u32 argumentThis)
    : m_owner(owner)
    , m_callInfo(callInfo)
    , m_argumentThis(argumentThis)
{
}

IntrospectionHint::~IntrospectionHint()
{
}

ConversionCost IntrospectionHint::calculateConversion(const Type& targetType) const
{
    return m_callInfo.overload->returnType.calculateConversion(targetType);
}

Value IntrospectionHint::call(const ArgInfo parameters[], u32 argumentCount) const
{
    return RTTI::call(m_callInfo, parameters, m_argumentThis, parameters + m_argumentThis,
                      argumentCount - m_argumentThis);
}

minitl::tuple< minitl::raw< const Method >, bool >
IntrospectionHint::getCall(DbContext& context) const
{
    raw< const Class > cls = m_callInfo.overload->returnType.metaclass;

    raw< const ObjectInfo > object = cls->getStaticProperty(Class::nameOperatorCall());
    if(object)
    {
        return minitl::make_tuple(object->value.as< raw< const Method > >(), false);
    }
    raw< const RTTI::Method > result = cls->getMethod(Class::nameOperatorCall());
    if(result) return minitl::make_tuple(result, true);
    if(cls->getProperty(Class::nameOperatorCall()))
        context.error(m_owner, Message::MessageType("call on object of type %s is dynamic")
                                   | m_callInfo.overload->returnType.name());
    return minitl::make_tuple(raw< const Method >::null(), false);
}

Type IntrospectionHint::getType() const
{
    return m_callInfo.overload->returnType;
}

bool IntrospectionHint::getPropertyType(DbContext& context, const istring name,
                                        Type& propertyType) const
{
    be_forceuse(context);
    const Type&                 type     = m_callInfo.overload->returnType;
    raw< const RTTI::Property > property = type.metaclass->getProperty(name);
    if(property)
    {
        propertyType = property->type;
        return true;
    }
    else
    {
        return false;
    }
}

}}}  // namespace BugEngine::RTTI::AST
