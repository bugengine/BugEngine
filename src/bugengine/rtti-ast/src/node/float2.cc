/* BugEngine <bugengine.devel@gmail.com>
   see LICENSE for detail */

#include <bugengine/rtti-ast/stdafx.h>
#include <bugengine/rtti-ast/node/float2.hh>

#include <bugengine/rtti-ast/dbcontext.hh>

namespace BugEngine { namespace RTTI { namespace AST {

Float2::Float2(float2 value) : Node(), m_value(value)
{
    be_forceuse(m_value);
}

Float2::~Float2()
{
}

ConversionCost Float2::distance(const Type& type) const
{
    return ConversionCalculator< float2 >::calculate(type);
}

bool Float2::isCompatible(DbContext& context, const RTTI::Type& expectedType) const
{
    if(!be_type< float2 >().isA(expectedType))
    {
        context.error(this,
                      Message::MessageType("cannot cast float2 value to %s") | expectedType.name());
        return false;
    }
    else
        return true;
}

void Float2::doEval(const RTTI::Type& expectedType, RTTI::Value& result) const
{
    be_forceuse(expectedType);
    result = RTTI::Value(m_value);
}

}}}  // namespace BugEngine::RTTI::AST
