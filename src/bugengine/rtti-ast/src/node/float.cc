/* BugEngine <bugengine.devel@gmail.com>
   see LICENSE for detail */

#include <bugengine/rtti-ast/stdafx.h>
#include <bugengine/rtti-ast/node/float.hh>

namespace BugEngine { namespace RTTI { namespace AST {

Float::Float(double value) : Node(), m_value(value)
{
    be_forceuse(m_value);
}

Float::~Float()
{
}

bool Float::isCompatible(const Type& expectedType) const
{
    return be_type< float >().isA(expectedType) || be_type< double >().isA(expectedType);
}

void Float::doEval(const RTTI::Type& expectedType, RTTI::Value& result) const
{
    be_assert(isCompatible(expectedType), "invalid conversion from float to %s" | expectedType);
    if(be_type< float >().isA(expectedType))
        result = RTTI::Value((float)m_value);
    else
        result = RTTI::Value((double)m_value);
}

}}}  // namespace BugEngine::RTTI::AST
