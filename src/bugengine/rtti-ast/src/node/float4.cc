/* BugEngine <bugengine.devel@gmail.com>
   see LICENSE for detail */

#include <bugengine/rtti-ast/stdafx.h>
#include <bugengine/rtti-ast/node/float4.hh>

#include <bugengine/rtti-ast/dbcontext.hh>

namespace BugEngine { namespace RTTI { namespace AST {

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

void Float4::doEval(const RTTI::Type& expectedType, RTTI::Value& result) const
{
    be_forceuse(expectedType);
    result = RTTI::Value(m_value);
}

void Float4::doVisit(Node::Visitor& visitor) const
{
    visitor.accept(this);
}

}}}  // namespace BugEngine::RTTI::AST
