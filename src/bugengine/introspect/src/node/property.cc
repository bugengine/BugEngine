/* BugEngine <bugengine.devel@gmail.com>
   see LICENSE for detail */

#include <bugengine/introspect/stdafx.h>
#include <bugengine/introspect/node/property.hh>

#include <bugengine/introspect/dbcontext.hh>
#include <bugengine/meta/engine/propertyinfo.script.hh>

namespace BugEngine { namespace Meta { namespace AST {

Property::Property(ref< const Object > owner, const inamespace& propertyName)
    : Node()
    , m_owner(owner)
    , m_propertyName(propertyName)
    , m_type()
{
}

Property::~Property()
{
}

ConversionCost Property::distance(const Type& type) const
{
    return m_type.calculateConversion(type);
}

minitl::tuple< raw< const Meta::Method >, bool > Property::getCall(DbContext& context) const
{
    be_forceuse(context);
    return minitl::make_tuple(raw< const Method >::null(), false);
}

bool Property::doResolve(DbContext& context)
{
    if(!m_owner->getPropertyType(context, m_propertyName[0], m_type))
    {
        context.error(this, Message::MessageType("type %s does not have a member %s")
                                | m_owner->getType().name() | m_propertyName[0]);
    }
    for(u32 i = 1; i < m_propertyName.size(); ++i)
    {
        raw< const Meta::Property > property = m_type.metaclass->getProperty(m_propertyName[i]);
        if(property)
        {
            m_type = property->type;
        }
        else
        {
            context.error(this, Message::MessageType("type %s does not have a member %s")
                                    | m_type.name() | m_propertyName[i]);
            return false;
        }
    }
    return true;
}

void Property::doEval(const Type& expectedType, Value& result) const
{
    be_forceuse(expectedType);
    result = m_owner->eval(expectedType);
    for(u32 i = 0; i < m_propertyName.size(); ++i)
    {
        bool  found;
        Value v = result.type().metaclass->get(result, m_propertyName[i], found);
        be_assert(found,
                  "type %s does not have a property %s" | result.type().name() | m_propertyName[i]);
        result.swap(v);
    }
    /* TODO: Policy */
    Value v(expectedType, result);
    result.swap(v);
}

void Property::doVisit(Node::Visitor& visitor) const
{
    visitor.accept(this);
}

}}}  // namespace BugEngine::Meta::AST
