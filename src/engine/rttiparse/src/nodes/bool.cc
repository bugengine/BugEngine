/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#include    <rttiparse/stdafx.h>
#include    <nodes/bool.hh>


namespace BugEngine { namespace RTTI { namespace Parser
{

Bool::Bool(const ParseLocation& location, bool value)
    :   Node(location)
    ,   m_value(value)
{
    be_forceuse(m_value);
}

Bool::~Bool()
{
}

bool Bool::isCompatible(const Type &expectedType) const
{
    return be_type<bool>().isA(expectedType);
}

void Bool::doEval(const RTTI::Type& expectedType, Value& result) const
{
    be_assert(isCompatible(expectedType), "invalid conversion from bool to %s" | expectedType);
    be_forceuse(expectedType);
    result = RTTI::Value(m_value);
}

}}}
