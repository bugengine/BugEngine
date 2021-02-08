/* BugEngine <bugengine.devel@gmail.com>
   see LICENSE for detail */

#include <bugengine/rtti-ast/stdafx.h>
#include <bugengine/rtti-ast/node/float4.hh>

namespace BugEngine { namespace RTTI { namespace AST {

Float4::Float4(float4 value) : Node(), m_value(value)
{
    be_forceuse(m_value);
}

Float4::~Float4()
{
}

bool Float4::isCompatible(const RTTI::Type& expectedType) const
{
    return be_type< float4 >().isA(expectedType);
}

void Float4::doEval(const RTTI::Type& expectedType, RTTI::Value& result) const
{
    be_assert(isCompatible(expectedType), "invalid conversion from float4 to %s" | expectedType);
    result = RTTI::Value(m_value);
}

}}}  // namespace BugEngine::RTTI::AST
