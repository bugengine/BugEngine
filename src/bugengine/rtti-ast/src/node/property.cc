/* BugEngine <bugengine.devel@gmail.com>
   see LICENSE for detail */

#include <bugengine/rtti-ast/stdafx.h>
#include <bugengine/rtti-ast/node/property.hh>

#include <bugengine/rtti-ast/dbcontext.hh>

namespace BugEngine { namespace RTTI { namespace AST {

Property::Property(weak< const Object > owner, const inamespace& propertyName)
    : Node()
    , m_owner(owner)
    , m_propertyName(propertyName)
{
}

Property::~Property()
{
}

ConversionCost Property::distance(const Type& type) const
{
    be_forceuse(type);
    return ConversionCost::s_incompatible;
}

minitl::tuple< raw< const RTTI::Method >, bool > Property::getCall(DbContext& context) const
{
    be_forceuse(context);
    return minitl::make_tuple(raw< const Method >::null(), false);
}

bool Property::doResolve(DbContext& context)
{
    be_forceuse(context);
    return true;
}

bool Property::isCompatible(DbContext& context, const Type& expectedType) const
{
    be_forceuse(context);
    be_forceuse(expectedType);
    return false;
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
}

}}}  // namespace BugEngine::RTTI::AST
