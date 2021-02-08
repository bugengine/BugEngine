/* BugEngine <bugengine.devel@gmail.com>
   see LICENSE for detail */

#include <bugengine/rtti-ast/stdafx.h>
#include <bugengine/rtti-ast/node/string.hh>

namespace BugEngine { namespace RTTI { namespace AST {

String::String(const char* value) : Node(), m_value(Arena::script().strdup(value))
{
}

String::~String()
{
}

bool String::isCompatible(const RTTI::Type& expectedType) const
{
    return be_type< istring >().isA(expectedType) || be_type< inamespace >().isA(expectedType);
}

void String::doEval(const RTTI::Type& expectedType, Value& result) const
{
    be_assert(isCompatible(expectedType), "invalid conversion from string to %s" | expectedType);
    if(be_type< istring >().isA(expectedType))
        result = RTTI::Value(istring(m_value));
    else if(be_type< inamespace >().isA(expectedType))
        result = RTTI::Value(inamespace(m_value));
}
}}}  // namespace BugEngine::RTTI::AST
