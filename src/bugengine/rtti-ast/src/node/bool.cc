/* BugEngine <bugengine.devel@gmail.com>
   see LICENSE for detail */

#include <bugengine/rtti-ast/stdafx.h>
#include <bugengine/rtti-ast/node/bool.hh>

#include <bugengine/rtti-ast/dbcontext.hh>

namespace BugEngine { namespace RTTI { namespace AST {

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

void Bool::doEval(const RTTI::Type& expectedType, Value& result) const
{
    be_forceuse(expectedType);
    result = RTTI::Value(m_value);
}

void Bool::doVisit(Node::Visitor& visitor) const
{
    visitor.accept(this);
}

}}}  // namespace BugEngine::RTTI::AST
