/* BugEngine <bugengine.devel@gmail.com>
   see LICENSE for detail */

#include <bugengine/introspect/stdafx.h>
#include <bugengine/introspect/node/float4.hh>

#include <bugengine/introspect/dbcontext.hh>

namespace BugEngine { namespace Meta { namespace AST {

Float4::Float4(float4 value) : Node(), m_value(value)
{
    be_forceuse(m_value);
}

Float4::~Float4()
{
}

ConversionCost Float4::distance(const Type& type) const
{
    return ConversionCalculator< float4 >::calculate(type);
}

void Float4::doEval(const Meta::Type& expectedType, Meta::Value& result) const
{
    be_forceuse(expectedType);
    result = Meta::Value(m_value);
}

void Float4::doVisit(Node::Visitor& visitor) const
{
    visitor.accept(this);
}

}}}  // namespace BugEngine::Meta::AST
