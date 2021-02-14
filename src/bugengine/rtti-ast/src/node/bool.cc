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
bool Bool::isCompatible(DbContext& context, const Type& expectedType) const
{
    if(!be_type< bool >().isA(expectedType))
    {
        context.error(this,
                      Message::MessageType("cannot cast bool value to %s") | expectedType.name());
        return false;
    }
    else
        return true;
}

void Bool::doEval(const RTTI::Type& expectedType, Value& result) const
{
    be_forceuse(expectedType);
    result = RTTI::Value(m_value);
}

}}}  // namespace BugEngine::RTTI::AST
