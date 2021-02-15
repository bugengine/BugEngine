/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#include <bugengine/rtti-ast/stdafx.h>
#include <bugengine/rtti-ast/node/reference.hh>

#include <bugengine/rtti-ast/dbcontext.hh>
#include <bugengine/rtti-ast/node/object.hh>

namespace BugEngine { namespace RTTI { namespace AST {

Reference::Reference(const inamespace& name) : Node(), m_referenceName(name)
{
}

Reference::~Reference()
{
}

bool Reference::doResolve(DbContext& context)
{
    weak< const Namespace > ns         = context.rootNamespace;
    inamespace              properties = m_referenceName;
    inamespace              current;
    while(properties.size())
    {
        istring                 n     = properties[0];
        weak< const Namespace > child = ns->getChild(n);
        if(child)
        {
            ns = child;
            properties.pop_front();
            if(properties.size() == 0) m_value = Value(Value::ByRef(ns->getValue()));
        }
        else
        {
            ref< const Node > node = ns->getNode(n);
            if(node)
            {
                m_node = node;
                properties.pop_front();
                if(properties.size() != 0)
                {
                    ref< Node > propertyNode = m_node->getProperty(context, properties);
                    if(propertyNode) propertyNode->resolve(context);
                    m_node = propertyNode;
                }
                break;
            }
            else
            {
                const Value& v = ns->getValue();
                if(!v)
                {
                    context.error(
                        this, Message::MessageType(
                                  "when resolving %s: namespace %s does not have a child named %s")
                                  | m_referenceName | current | n);
                    return false;
                }
                else
                {
                    m_value = Value(Value::ByRef(v));
                    while(properties.size())
                    {
                        n = properties.pop_front();
                        bool  found;
                        Type  t = v.type();
                        Value v = t.metaclass->get(m_value, n, found);
                        m_value.swap(v);
                        if(!found)
                        {
                            context.error(
                                this, Message::MessageType("when resolving %s: object %s of type "
                                                           "%s does not have a property named %s")
                                          | m_referenceName | current | t.name() | n);
                            return false;
                        }
                        current.push_back(n);
                    }
                }
                break;
            }
        }
        current.push_back(n);
    }
    return true;
}

ConversionCost Reference::distance(const Type& type) const
{
    if(m_node)
    {
        return m_node->distance(type);
    }
    else
    {
        return m_value.type().calculateConversion(type);
    }
}

bool Reference::isCompatible(DbContext& context, const Type& expectedType) const
{
    if(distance(expectedType) >= ConversionCost::s_incompatible)
    {
        context.error(this, Message::MessageType("cannot cast reference to %s value to %s")
                                | m_referenceName | expectedType.name());
        return false;
    }
    else
        return true;
}

void Reference::doEval(const Type& expectedType, Value& result) const
{
    if(m_node)
    {
        m_node->eval(expectedType, result);
    }
    else
    {
        result = m_value;
    }
}

minitl::tuple< raw< const RTTI::Method >, bool > Reference::getCall(DbContext& context) const
{
    be_forceuse(context);
    if(m_node)
        return minitl::make_tuple(raw< const Method >::null(), false);
    else
    {
        bool                    found     = false;
        raw< const Class >      metaclass = m_value.type().metaclass;
        raw< const ObjectInfo > staticCall
            = metaclass->getStaticProperty(Class::nameOperatorCall());
        if(staticCall)
        {
            return minitl::make_tuple(staticCall->value.as< raw< const Method > >(), false);
        }
        raw< const Method > method = metaclass->getMethod(Class::nameOperatorCall());
        if(method)
        {
            return minitl::make_tuple(method, true);
        }
        Value call = metaclass->get(m_value, Class::nameOperatorCall(), found);
        if(found)
        {
            return minitl::make_tuple(call.as< raw< const Method > >(), false);
        }
        else
        {
            return minitl::make_tuple(raw< const Method >::null(), false);
        }
    }
}

}}}  // namespace BugEngine::RTTI::AST
