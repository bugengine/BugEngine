/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#include    <rttiparse/stdafx.h>
#include    <nodes/float.hh>


namespace BugEngine { namespace RTTI { namespace Parser
{

Float::Float(const ParseLocation& location, double value)
    :   Node(location)
    ,   m_value(value)
{
    be_forceuse(m_value);
}

Float::~Float()
{
}

bool Float::isCompatible(const Type &expectedType) const
{
    return be_type<float>().isA(expectedType)
        || be_type<double>().isA(expectedType);
}

void Float::doEval(const RTTI::Type& expectedType, RTTI::Value& result) const
{
    be_assert(isCompatible(expectedType), "invalid conversion from float to %s" | expectedType);
    if (be_type<float>().isA(expectedType))
        result = RTTI::Value((float)m_value);
    else
        result = RTTI::Value((double)m_value);
}

}}}
