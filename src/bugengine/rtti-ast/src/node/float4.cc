/* BugEngine <bugengine.devel@gmail.com>
   see LICENSE for detail */

#include <bugengine/rtti-ast/stdafx.h>
#include <bugengine/rtti-ast/node/float4.hh>

#include <bugengine/rtti-ast/dbcontext.hh>

namespace BugEngine { namespace RTTI { namespace AST {

Float4::Float4(float4 value) : Node(), m_value(value)
{
    be_forceuse(m_value);
}

Float4::~Float4()
{
}

ConversionCost Float4::distance(const Type& type) const
{
    return ConversionCalculator< float4 >::calculate(type);
}

bool Float4::isCompatible(DbContext& context, const RTTI::Type& expectedType) const
{
    if(!be_type< float4 >().isA(expectedType))
    {
        context.error(this,
                      Message::MessageType("cannot cast float4 value to %s") | expectedType.name());
        return false;
    }
    else
        return true;
}

void Float4::doEval(const RTTI::Type& expectedType, RTTI::Value& result) const
{
    be_forceuse(expectedType);
    result = RTTI::Value(m_value);
}

}}}  // namespace BugEngine::RTTI::AST
