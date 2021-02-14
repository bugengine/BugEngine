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

bool Float::isCompatible(DbContext& context, const Type& expectedType) const
{
    if(distance(expectedType) >= ConversionCost::s_incompatible)
    {
        context.error(this,
                      Message::MessageType("cannot cast float value to %s") | expectedType.name());
        return false;
    }
    else
        return true;
}

void Float::doEval(const RTTI::Type& expectedType, RTTI::Value& result) const
{
    if(be_type< float >().isA(expectedType))
        result = RTTI::Value((float)m_value);
    else
        result = RTTI::Value((double)m_value);
}

}}}  // namespace BugEngine::RTTI::AST
