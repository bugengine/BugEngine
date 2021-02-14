/* BugEngine <bugengine.devel@gmail.com>
   see LICENSE for detail */

#include <bugengine/rtti-ast/stdafx.h>
#include <bugengine/rtti-ast/node/object.hh>
#include <bugengine/rtti-ast/node/reference.hh>

#include <bugengine/rtti-ast/dbcontext.hh>
#include <bugengine/rtti/classinfo.script.hh>
#include <bugengine/rtti/engine/call.hh>
#include <bugengine/rtti/engine/methodinfo.script.hh>
#include <bugengine/rtti/engine/objectinfo.script.hh>
#include <bugengine/rtti/value.hh>

namespace BugEngine { namespace RTTI { namespace AST {

RTTI::ConversionCost calculateConversion(weak< const Node > node, const RTTI::Type& other)
{
    return node->distance(other);
}

void convert(weak< const Node > node, void* buffer, RTTI::Type type)
{
    // return node->unpack(buffer, type);
    new(buffer) Value(node->eval(type));
}

Object::Object(ref< Reference > className, const minitl::vector< Parameter >& parameters)
    : Node()
    , m_className(className)
    , m_parameters(parameters)
    , m_arguments(Arena::temporary())
    , m_callInfo()
    , m_argumentThis()
{
}

Object::~Object()
{
}

ConversionCost Object::distance(const Type& type) const
{
    if(m_callInfo.overload)
    {
        return m_callInfo.overload->returnType.calculateConversion(type);
    }
    else
    {
        return ConversionCost::s_incompatible;
    }
}

minitl::tuple< raw< const RTTI::Method >, bool > Object::getCall(DbContext& context) const
{
    if(m_callInfo.overload)
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
            context.error(this, Message::MessageType("call on object of type %s is dynamic")
                                    | m_callInfo.overload->returnType.name());
        return minitl::make_tuple(raw< const Method >::null(), false);
    }
    else
    {
        return minitl::make_tuple(raw< const Method >::null(), false);
    }
}

bool Object::doResolve(DbContext& context)
{
    bool result = m_className->resolve(context);

    for(minitl::vector< Parameter >::const_iterator it = m_parameters.begin();
        it != m_parameters.end(); ++it)
    {
        result &= it->value->resolve(context);
    }
    if(result)
    {
        minitl::tuple< raw< const RTTI::Method >, bool > method = m_className->getCall(context);
        if(!method.first)
        {
            context.error(this,
                          Message::MessageType("unable to call object %s") | m_className->name());
        }
        else
        {
            m_argumentThis    = method.second ? 1 : 0;
            u32 argumentCount = m_parameters.size() + m_argumentThis;
            m_arguments.resize(argumentCount);
            if(method.second)
            {
                m_arguments[0] = ArgInfo(this);
            }
            for(u32 currentArg = 0; currentArg < m_parameters.size(); ++currentArg)
            {
                m_arguments[currentArg + m_argumentThis]
                    = ArgInfo(m_parameters[currentArg].name, m_parameters[currentArg].value);
            }

            ArgInfo* arguments = &m_arguments.front();
            m_callInfo         = RTTI::resolve(method.first, arguments, m_argumentThis,
                                       arguments + m_argumentThis, m_parameters.size());
        }
    }
    return result;
}

bool Object::isCompatible(DbContext& context, const Type& expectedType) const
{
    if(m_callInfo.overload)
    {
        if(!(expectedType <= m_callInfo.overload->returnType))
        {
            context.error(this, Message::MessageType("cannot cast %s value to %s")
                                    | m_callInfo.overload->returnType.name() | expectedType.name());
            return false;
        }
        else
        {
            return true;
        }
    }
    else
    {
        return false;
    }
}

void Object::doEval(const Type& expectedType, Value& result) const
{
    be_forceuse(expectedType);
    const ArgInfo* arguments = &m_arguments.front();
    result = RTTI::call(m_callInfo, arguments, m_argumentThis, arguments + m_argumentThis,
                        m_arguments.size() - m_argumentThis);
}

Type Object::getType() const
{
    return be_type< void >();
}

}}}  // namespace BugEngine::RTTI::AST
