/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#include <bugengine/rtti-ast/stdafx.h>
#include <bugengine/rtti-ast/node/float3.hh>

namespace BugEngine { namespace RTTI { namespace AST {

Float3::Float3(const ParseLocation& location, float3 value) : Node(location), m_value(value)
{
    be_forceuse(m_value);
}

Float3::~Float3()
{
}

bool Float3::isCompatible(const RTTI::Type& expectedType) const
{
    return be_type< float3 >().isA(expectedType);
}

void Float3::doEval(const RTTI::Type& expectedType, RTTI::Value& result) const
{
    be_assert(isCompatible(expectedType), "invalid conversion from float3 to %s" | expectedType);
    result = RTTI::Value(m_value);
}

}}}  // namespace BugEngine::RTTI::AST
