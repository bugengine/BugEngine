/* BugEngine <bugengine.devel@gmail.com>
   see LICENSE for detail */

#include <bugengine/introspect/stdafx.h>
#include <bugengine/introspect/node/string.hh>

#include <bugengine/introspect/dbcontext.hh>

namespace BugEngine { namespace Meta { namespace AST {

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

void String::doEval(const Meta::Type& expectedType, Value& result) const
{
    if(be_type< istring >().isA(expectedType))
        result = Meta::Value(istring(m_value));
    else if(be_type< inamespace >().isA(expectedType))
        result = Meta::Value(inamespace(m_value));
    else
        be_notreached();
}

void String::doVisit(Node::Visitor& visitor) const
{
    visitor.accept(this);
}

}}}  // namespace BugEngine::Meta::AST
