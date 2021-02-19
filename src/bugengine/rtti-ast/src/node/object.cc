/* BugEngine <bugengine.devel@gmail.com>
   see LICENSE for detail */

#include <bugengine/rtti-ast/stdafx.h>
#include <bugengine/rtti-ast/node/object.hh>

#include <bugengine/rtti-ast/dbcontext.hh>
#include <bugengine/rtti-ast/introspectionhint.hh>
#include <bugengine/rtti-ast/node/parameter.hh>
#include <bugengine/rtti-ast/node/property.hh>
#include <bugengine/rtti-ast/node/reference.hh>
#include <bugengine/rtti-ast/policy.script.hh>

#include <bugengine/rtti/classinfo.script.hh>
#include <bugengine/rtti/engine/call.hh>
#include <bugengine/rtti/value.hh>

namespace BugEngine { namespace RTTI { namespace AST {

Object::Object(ref< Reference > className, const minitl::vector< ref< Parameter > >& parameters)
    : Node()
    , m_className(className)
    , m_parameters(parameters)
    , m_arguments(Arena::temporary())
{
}

Object::~Object()
{
}

ConversionCost Object::distance(const Type& type) const
{
    if(m_introspectionHint)
    {
        return m_introspectionHint->calculateConversion(type);
    }
    else
    {
        return ConversionCost::s_incompatible;
    }
}

minitl::tuple< raw< const RTTI::Method >, bool > Object::getCall(DbContext& context) const
{
    if(m_introspectionHint)
    {
        return m_introspectionHint->getCall(context);
    }
    else
    {
        return minitl::make_tuple(raw< const Method >::null(), false);
    }
}

bool Object::doResolve(DbContext& context)
{
    be_assert(!m_introspectionHint,
              "object has already been resolved; make sure to reset the state first");
    bool result = resolveInternal(context);
    if(result)
    {
        be_assert(m_introspectionHint, "object resolution succeeded without providing a hint");
    }
    else
    {
        be_assert_recover(!m_introspectionHint, "object resolution failed, but produced a hint",
                          m_introspectionHint.clear(););
    }
    return result;
}

bool Object::resolveInternal(DbContext& context)
{
    bool result = m_className->resolve(context);

    for(minitl::vector< ref< Parameter > >::const_iterator it = m_parameters.begin();
        it != m_parameters.end(); ++it)
    {
        result &= (*it)->resolve(context);
    }
    if(result)
    {
        minitl::tuple< raw< const RTTI::Method >, bool > method = m_className->getCall(context);
        if(!method.first)
        {
            context.error(this,
                          Message::MessageType("unable to call object %s") | m_className->name());
            result = false;
        }
        else
        {
            u32 argumentThis  = method.second ? 1 : 0;
            u32 argumentCount = m_parameters.size() + argumentThis;
            m_arguments.resize(argumentCount);
            if(method.second)
            {
                m_arguments[0] = ArgInfo(this);
            }
            for(u32 currentArg = 0; currentArg < m_parameters.size(); ++currentArg)
            {
                m_arguments[currentArg + argumentThis]
                    = ArgInfo(m_parameters[currentArg]->name(), m_parameters[currentArg]);
            }

            ArgInfo* arguments = m_arguments.empty() ? 0 : &m_arguments.front();
            CallInfo callInfo  = RTTI::resolve(method.first, arguments, argumentThis,
                                              arguments + argumentThis, m_parameters.size());
            if(callInfo.overload)
            {
                RTTI::Value policyTag = callInfo.overload->getTag(be_class< Policy >());
                if(policyTag)
                {
                    const Policy& policy     = policyTag.as< const Policy& >();
                    u32           errorCount = context.errorCount;
                    m_introspectionHint      = policy.verify(context, this, callInfo, argumentThis);
                    result                   = errorCount == context.errorCount;
                }
                else
                {
                    m_introspectionHint = ref< IntrospectionHint >::create(Arena::rtti(), this,
                                                                           callInfo, argumentThis);
                }
            }
            else
            {
                result = false;
                context.error(
                    this, Message::MessageType(
                              "unable to call object %s: no overload could convert all arguments")
                              | m_className->name());
            }
        }
    }
    return result;
}

void Object::doEval(const Type& expectedType, Value& result) const
{
    be_forceuse(expectedType);
    const ArgInfo* arguments = m_arguments.empty() ? 0 : &m_arguments.front();
    result                   = m_introspectionHint->call(arguments, m_arguments.size());
}

Type Object::getType() const
{
    return m_introspectionHint->getType();
}

bool Object::getPropertyType(DbContext& context, const istring propertyName,
                             Type& propertyType) const
{
    return m_introspectionHint->getPropertyType(context, propertyName, propertyType);
}

ref< Node > Object::getProperty(DbContext& context, const inamespace& propertyName) const
{
    be_forceuse(context);
    return ref< Property >::create(Arena::general(), refFromThis< Object >(), propertyName);
}

weak< const Parameter > Object::getParameter(istring parameterName) const
{
    for(minitl::vector< ref< Parameter > >::const_iterator it = m_parameters.begin();
        it != m_parameters.end(); ++it)
    {
        if((*it)->name() == parameterName) return *it;
    }
    return weak< const Parameter >();
}

void Object::doVisit(Node::Visitor& visitor) const
{
    visitor.accept(this);
}

}}}  // namespace BugEngine::RTTI::AST
