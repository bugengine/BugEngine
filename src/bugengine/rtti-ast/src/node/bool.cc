/* BugEngine <bugengine.devel@gmail.com>
   see LICENSE for detail */

#include <bugengine/rtti-ast/stdafx.h>
#include <bugengine/rtti-ast/node/bool.hh>

namespace BugEngine { namespace RTTI { namespace AST {

Bool::Bool(bool value) : Node(), m_value(value)
{
    be_forceuse(m_value);
}

Bool::~Bool()
{
}

bool Bool::isCompatible(const Type& expectedType) const
{
    return be_type< bool >().isA(expectedType);
}

void Bool::doEval(const RTTI::Type& expectedType, Value& result) const
{
    be_assert(isCompatible(expectedType), "invalid conversion from bool to %s" | expectedType);
    be_forceuse(expectedType);
    result = RTTI::Value(m_value);
}

}}}  // namespace BugEngine::RTTI::AST
