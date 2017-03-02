/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#include    <rttiparse/stdafx.h>
#include    <nodes/int2.hh>


namespace BugEngine { namespace RTTI { namespace Parser
{

Int2::Int2(const ParseLocation& location, bigint2 value)
    :   Node(location)
    ,   m_value(value)
{
    be_forceuse(m_value);
}

Int2::~Int2()
{
}

bool Int2::isCompatible(const RTTI::Type& expectedType) const
{
    return be_typeid<byte2>::type().isA(expectedType)
        || be_typeid<short2>::type().isA(expectedType)
        || be_typeid<int2>::type().isA(expectedType)
        || be_typeid<bigint2>::type().isA(expectedType)
        || be_typeid<ushort2>::type().isA(expectedType)
        || be_typeid<uint2>::type().isA(expectedType)
        || be_typeid<biguint2>::type().isA(expectedType);
}

void Int2::doEval(const RTTI::Type& expectedType, Value& result) const
{
    be_assert(isCompatible(expectedType), "invalid conversion from int2 to %s" | expectedType);
    if (be_typeid<byte2>::type().isA(expectedType))
        result = RTTI::Value(make_byte2(be_checked_numcast<i8>(m_value[0]),
                                        be_checked_numcast<i8>(m_value[1])));
    else if (be_typeid<short2>::type().isA(expectedType))
        result =  RTTI::Value(make_short2(be_checked_numcast<i16>(m_value[0]),
                                          be_checked_numcast<i16>(m_value[1])));
    else if (be_typeid<int2>::type().isA(expectedType))
        result =  RTTI::Value(make_int2(be_checked_numcast<i32>(m_value[0]),
                                        be_checked_numcast<i32>(m_value[1])));
    else if (be_typeid<bigint2>::type().isA(expectedType))
        result =  RTTI::Value(make_bigint2(be_checked_numcast<i64>(m_value[0]),
                                           be_checked_numcast<i64>(m_value[1])));
    else if (be_typeid<ushort2>::type().isA(expectedType))
        result =  RTTI::Value(make_ushort2(be_checked_numcast<u16>(m_value[0]),
                                           be_checked_numcast<u16>(m_value[1])));
    else if (be_typeid<uint2>::type().isA(expectedType))
        result =  RTTI::Value(make_uint2(be_checked_numcast<u32>(m_value[0]),
                                         be_checked_numcast<u32>(m_value[1])));
    else if (be_typeid<biguint2>::type().isA(expectedType))
        result =  RTTI::Value(make_biguint2(be_checked_numcast<u64>(m_value[0]),
                                            be_checked_numcast<u64>(m_value[1])));
}

}}}
