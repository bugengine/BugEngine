/* BugEngine <bugengine.devel@gmail.com>
   see LICENSE for detail */

#include <bugengine/introspect/stdafx.h>
#include <bugengine/introspect/node/float3.hh>

#include <bugengine/introspect/dbcontext.hh>

namespace BugEngine { namespace Meta { namespace AST {

Float3::Float3(float3 value) : Node(), m_value(value)
{
    be_forceuse(m_value);
}

Float3::~Float3()
{
}

ConversionCost Float3::distance(const Type& type) const
{
    return ConversionCalculator< float3 >::calculate(type);
}

void Float3::doEval(const Meta::Type& expectedType, Meta::Value& result) const
{
    be_forceuse(expectedType);
    result = Meta::Value(m_value);
}

void Float3::doVisit(Node::Visitor& visitor) const
{
    visitor.accept(this);
}

}}}  // namespace BugEngine::Meta::AST
