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
    weak< const Namespace > ns = context.rootNamespace;
    m_properties               = m_referenceName;
    inamespace current;
    while(m_properties.size())
    {
        istring                 n     = m_properties[0];
        weak< const Namespace > child = ns->getChild(n);
        if(child)
        {
            ns = child;
            m_properties.pop_front();
        }
        else
        {
            weak< const Node > node = ns->getNode(n);
            if(!node)
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
                    while(m_properties.size())
                    {
                        n = m_properties.pop_front();
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
    be_forceuse(context);
    be_forceuse(expectedType);
    return true;
}

void Reference::doEval(const Type& expectedType, Value& result) const
{
    be_forceuse(expectedType);
    be_forceuse(result);
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
