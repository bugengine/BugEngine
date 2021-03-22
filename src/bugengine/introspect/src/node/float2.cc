/* BugEngine <bugengine.devel@gmail.com>
   see LICENSE for detail */

#include <bugengine/introspect/stdafx.h>
#include <bugengine/introspect/node/float2.hh>

#include <bugengine/introspect/dbcontext.hh>

namespace BugEngine { namespace Meta { namespace AST {

Float2::Float2(float2 value) : Node(), m_value(value)
{
    be_forceuse(m_value);
}

Float2::~Float2()
{
}

ConversionCost Float2::distance(const Type& type) const
{
    return ConversionCalculator< float2 >::calculate(type);
}

void Float2::doEval(const Meta::Type& expectedType, Meta::Value& result) const
{
    be_forceuse(expectedType);
    result = Meta::Value(m_value);
}

void Float2::doVisit(Node::Visitor& visitor) const
{
    visitor.accept(this);
}

}}}  // namespace BugEngine::Meta::AST
