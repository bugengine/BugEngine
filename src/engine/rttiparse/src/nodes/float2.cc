/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#include    <rttiparse/stdafx.h>
#include    <nodes/float2.hh>


namespace BugEngine { namespace RTTI { namespace Parser
{

Float2::Float2(const ParseLocation& location, float2 value)
    :   Node(location)
    ,   m_value(value)
{
    be_forceuse(m_value);
}

Float2::~Float2()
{
}

bool Float2::isCompatible(const RTTI::Type& expectedType) const
{
    return be_typeid<float2>::type().isA(expectedType);
}

void Float2::doEval(const RTTI::Type& expectedType, RTTI::Value& result) const
{
    be_assert(isCompatible(expectedType), "invalid conversion from float2 to %s" | expectedType);
    result = RTTI::Value(m_value);
}

}}}
