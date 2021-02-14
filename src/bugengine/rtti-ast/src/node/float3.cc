/* BugEngine <bugengine.devel@gmail.com>
   see LICENSE for detail */

#include <bugengine/rtti-ast/stdafx.h>
#include <bugengine/rtti-ast/node/float3.hh>

#include <bugengine/rtti-ast/dbcontext.hh>

namespace BugEngine { namespace RTTI { namespace AST {

Float3::Float3(float3 value) : Node(), m_value(value)
{
    be_forceuse(m_value);
}

Float3::~Float3()
{
}

ConversionCost Float3::distance(const Type& type) const
{
    return ConversionCalculator< float3 >::calculate(type);
}

bool Float3::isCompatible(DbContext& context, const RTTI::Type& expectedType) const
{
    if(!be_type< float3 >().isA(expectedType))
    {
        context.error(this,
                      Message::MessageType("cannot cast float3 value to %s") | expectedType.name());
        return false;
    }
    else
        return true;
}

void Float3::doEval(const RTTI::Type& expectedType, RTTI::Value& result) const
{
    be_forceuse(expectedType);
    result = RTTI::Value(m_value);
}

}}}  // namespace BugEngine::RTTI::AST
