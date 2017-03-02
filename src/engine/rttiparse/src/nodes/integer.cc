/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#include    <rttiparse/stdafx.h>
#include    <nodes/integer.hh>


namespace BugEngine { namespace RTTI { namespace Parser
{

Integer::Integer(const ParseLocation& location, i64 value)
    :   Node(location)
    ,   m_value(value)
{
    be_forceuse(m_value);
}

Integer::~Integer()
{
}

bool Integer::isCompatible(const RTTI::Type& expectedType) const
{
    return be_typeid<i8>::type().isA(expectedType)
        || be_typeid<i16>::type().isA(expectedType)
        || be_typeid<i32>::type().isA(expectedType)
        || be_typeid<i64>::type().isA(expectedType)
        || be_typeid<u8>::type().isA(expectedType)
        || be_typeid<u16>::type().isA(expectedType)
        || be_typeid<u32>::type().isA(expectedType)
        || be_typeid<u64>::type().isA(expectedType);
}

void Integer::doEval(const RTTI::Type& expectedType, RTTI::Value& result) const
{
    be_assert(isCompatible(expectedType), "invalid conversion from int to %s" | expectedType);
    if (be_typeid<i8>::type().isA(expectedType))
        result = RTTI::Value(be_checked_numcast<i8>(m_value));
    else if (be_typeid<i16>::type().isA(expectedType))
        result = RTTI::Value(be_checked_numcast<i16>(m_value));
    else if (be_typeid<i32>::type().isA(expectedType))
        result = RTTI::Value(be_checked_numcast<i32>(m_value));
    else if (be_typeid<i64>::type().isA(expectedType))
        result = RTTI::Value(be_checked_numcast<i64>(m_value));
    else if (be_typeid<u8>::type().isA(expectedType))
        result = RTTI::Value(be_checked_numcast<u8>(m_value));
    else if (be_typeid<u16>::type().isA(expectedType))
        result = RTTI::Value(be_checked_numcast<u16>(m_value));
    else if (be_typeid<u32>::type().isA(expectedType))
        result = RTTI::Value(be_checked_numcast<u32>(m_value));
    else if (be_typeid<u64>::type().isA(expectedType))
        result = RTTI::Value(be_checked_numcast<u64>(m_value));
}

}}}
