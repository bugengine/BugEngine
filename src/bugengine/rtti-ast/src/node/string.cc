/* BugEngine <bugengine.devel@gmail.com>
   see LICENSE for detail */

#include <bugengine/rtti-ast/stdafx.h>
#include <bugengine/rtti-ast/node/string.hh>

#include <bugengine/rtti-ast/dbcontext.hh>

namespace BugEngine { namespace RTTI { namespace AST {

String::String(const char* value) : Node(), m_value(Arena::script().strdup(value))
{
}

String::~String()
{
}

ConversionCost String::distance(const Type& type) const
{
    if(type.metaclass->type() == ClassType_String)
        return ConversionCost();
    else
        return ConversionCost::s_incompatible;
}

void String::doEval(const RTTI::Type& expectedType, Value& result) const
{
    if(be_type< istring >().isA(expectedType))
        result = RTTI::Value(istring(m_value));
    else if(be_type< inamespace >().isA(expectedType))
        result = RTTI::Value(inamespace(m_value));
    else
        be_notreached();
}

void String::doVisit(Node::Visitor& visitor) const
{
    visitor.accept(this);
}

}}}  // namespace BugEngine::RTTI::AST
