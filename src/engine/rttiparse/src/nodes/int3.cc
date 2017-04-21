/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#include    <rttiparse/stdafx.h>
#include    <nodes/int3.hh>


namespace BugEngine { namespace RTTI { namespace Parser
{

Int3::Int3(const ParseLocation& location, bigint3 value)
    :   Node(location)
    ,   m_value(value)
{
    be_forceuse(m_value);
}

Int3::~Int3()
{
}

bool Int3::isCompatible(const RTTI::Type& expectedType) const
{
    return be_typeid<byte3>::type().isA(expectedType)
        || be_typeid<short3>::type().isA(expectedType)
        || be_typeid<int3>::type().isA(expectedType)
        || be_typeid<bigint3>::type().isA(expectedType)
        || be_typeid<ushort3>::type().isA(expectedType)
        || be_typeid<uint3>::type().isA(expectedType)
        || be_typeid<biguint3>::type().isA(expectedType);
}

void Int3::doEval(const RTTI::Type& expectedType, Value& result) const
{
    be_assert(isCompatible(expectedType), "invalid conversion from int3 to %s" | expectedType);
    if (be_typeid<byte3>::type().isA(expectedType))
        result = RTTI::Value(make_byte3(be_checked_numcast<i8>(m_value[0]),
                                        be_checked_numcast<i8>(m_value[1]),
                                        be_checked_numcast<i8>(m_value[2])));
    else if (be_typeid<short3>::type().isA(expectedType))
        result =  RTTI::Value(make_short3(be_checked_numcast<i16>(m_value[0]),
                                          be_checked_numcast<i16>(m_value[1]),
                                          be_checked_numcast<i16>(m_value[2])));
    else if (be_typeid<int3>::type().isA(expectedType))
        result =  RTTI::Value(make_int3(be_checked_numcast<i32>(m_value[0]),
                                        be_checked_numcast<i32>(m_value[1]),
                                        be_checked_numcast<i32>(m_value[2])));
    else if (be_typeid<bigint3>::type().isA(expectedType))
        result =  RTTI::Value(make_bigint3(be_checked_numcast<i64>(m_value[0]),
                                           be_checked_numcast<i64>(m_value[1]),
                                           be_checked_numcast<i64>(m_value[2])));
    else if (be_typeid<ushort3>::type().isA(expectedType))
        result =  RTTI::Value(make_ushort3(be_checked_numcast<u16>(m_value[0]),
                                           be_checked_numcast<u16>(m_value[1]),
                                           be_checked_numcast<u16>(m_value[2])));
    else if (be_typeid<uint3>::type().isA(expectedType))
        result =  RTTI::Value(make_uint3(be_checked_numcast<u32>(m_value[0]),
                                         be_checked_numcast<u32>(m_value[1]),
                                         be_checked_numcast<u32>(m_value[2])));
    else if (be_typeid<biguint3>::type().isA(expectedType))
        result =  RTTI::Value(make_biguint3(be_checked_numcast<u64>(m_value[0]),
                                            be_checked_numcast<u64>(m_value[1]),
                                            be_checked_numcast<u64>(m_value[2])));
}


}}}
