/* BugEngine <bugengine.devel@gmail.com>
   see LICENSE for detail */

#include <bugengine/rtti-ast/stdafx.h>
#include <bugengine/rtti-ast/node/float2.hh>

#include <bugengine/rtti-ast/dbcontext.hh>

namespace BugEngine { namespace RTTI { namespace AST {

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

void Float2::doEval(const RTTI::Type& expectedType, RTTI::Value& result) const
{
    be_forceuse(expectedType);
    result = RTTI::Value(m_value);
}

void Float2::doVisit(Node::Visitor& visitor) const
{
    visitor.accept(this);
}

}}}  // namespace BugEngine::RTTI::AST
