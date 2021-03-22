/* BugEngine <bugengine.devel@gmail.com>
   see LICENSE for detail */

#include <bugengine/introspect/stdafx.h>
#include <bugengine/introspect/node/float.hh>

#include <bugengine/introspect/dbcontext.hh>

namespace BugEngine { namespace Meta { namespace AST {

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

void Float::doEval(const Meta::Type& expectedType, Meta::Value& result) const
{
    if(be_type< float >().isA(expectedType))
        result = Meta::Value((float)m_value);
    else
        result = Meta::Value((double)m_value);
}

void Float::doVisit(Node::Visitor& visitor) const
{
    visitor.accept(this);
}

}}}  // namespace BugEngine::Meta::AST
