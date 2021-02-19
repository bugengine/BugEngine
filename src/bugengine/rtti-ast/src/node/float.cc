/* BugEngine <bugengine.devel@gmail.com>
   see LICENSE for detail */

#include <bugengine/rtti-ast/stdafx.h>
#include <bugengine/rtti-ast/node/float.hh>

#include <bugengine/rtti-ast/dbcontext.hh>

namespace BugEngine { namespace RTTI { namespace AST {

Float::Float(double value) : Node(), m_value(value)
{
    be_forceuse(m_value);
}

Float::~Float()
{
}

ConversionCost Float::distance(const Type& type) const
{
    return ConversionCalculator< double >::calculate(type);
}

void Float::doEval(const RTTI::Type& expectedType, RTTI::Value& result) const
{
    if(be_type< float >().isA(expectedType))
        result = RTTI::Value((float)m_value);
    else
        result = RTTI::Value((double)m_value);
}

void Float::doVisit(Node::Visitor& visitor) const
{
    visitor.accept(this);
}

}}}  // namespace BugEngine::RTTI::AST
