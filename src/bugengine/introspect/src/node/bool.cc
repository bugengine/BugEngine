/* BugEngine <bugengine.devel@gmail.com>
   see LICENSE for detail */

#include <bugengine/introspect/stdafx.h>
#include <bugengine/introspect/node/bool.hh>

#include <bugengine/introspect/dbcontext.hh>

namespace BugEngine { namespace Meta { namespace AST {

Bool::Bool(bool value) : Node(), m_value(value)
{
    be_forceuse(m_value);
}

Bool::~Bool()
{
}

ConversionCost Bool::distance(const Type& type) const
{
    return ConversionCalculator< bool >::calculate(type);
}

void Bool::doEval(const Meta::Type& expectedType, Value& result) const
{
    be_forceuse(expectedType);
    result = Meta::Value(m_value);
}

void Bool::doVisit(Node::Visitor& visitor) const
{
    visitor.accept(this);
}

}}}  // namespace BugEngine::Meta::AST
