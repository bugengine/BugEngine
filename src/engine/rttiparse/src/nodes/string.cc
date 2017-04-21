/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#include    <rttiparse/stdafx.h>
#include    <nodes/string.hh>


namespace BugEngine { namespace RTTI { namespace Parser
{

String::String(const ParseLocation& location, const char* value)
    :   Node(location)
    ,   m_value(Arena::script().strdup(value))
{
}

String::~String()
{
}

bool String::isCompatible(const RTTI::Type& expectedType) const
{
    return be_typeid<istring>::type().isA(expectedType)
        || be_typeid<inamespace>::type().isA(expectedType);
}

void String::doEval(const RTTI::Type& expectedType, Value& result) const
{
    be_assert(isCompatible(expectedType), "invalid conversion from string to %s" | expectedType);
    if (be_typeid< istring >::type().isA(expectedType))
        result = RTTI::Value(istring(m_value));
    else if (be_typeid< inamespace >::type().isA(expectedType))
        result = RTTI::Value(inamespace(m_value));
}
}}}
