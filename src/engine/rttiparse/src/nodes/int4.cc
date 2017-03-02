/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#include    <rttiparse/stdafx.h>
#include    <nodes/int4.hh>


namespace BugEngine { namespace RTTI { namespace Parser
{

Int4::Int4(const ParseLocation& location, bigint4 value)
    :   Node(location)
    ,   m_value(value)
{
    be_forceuse(m_value);
}

Int4::~Int4()
{
}

bool Int4::isCompatible(const RTTI::Type& expectedType) const
{
    return be_typeid<byte4>::type().isA(expectedType)
        || be_typeid<short4>::type().isA(expectedType)
        || be_typeid<int4>::type().isA(expectedType)
        || be_typeid<bigint4>::type().isA(expectedType)
        || be_typeid<ushort4>::type().isA(expectedType)
        || be_typeid<uint4>::type().isA(expectedType)
        || be_typeid<biguint4>::type().isA(expectedType);
}

void Int4::doEval(const RTTI::Type& expectedType, Value& result) const
{
    be_assert(isCompatible(expectedType), "invalid conversion from int4 to %s" | expectedType);
    if (be_typeid<byte4>::type().isA(expectedType))
        result = RTTI::Value(make_byte4(be_checked_numcast<i8>(m_value[0]),
                                        be_checked_numcast<i8>(m_value[1]),
                                        be_checked_numcast<i8>(m_value[2]),
                                        be_checked_numcast<i8>(m_value[3])));
    else if (be_typeid<short4>::type().isA(expectedType))
        result =  RTTI::Value(make_short4(be_checked_numcast<i16>(m_value[0]),
                                          be_checked_numcast<i16>(m_value[1]),
                                          be_checked_numcast<i16>(m_value[2]),
                                          be_checked_numcast<i16>(m_value[3])));
    else if (be_typeid<int4>::type().isA(expectedType))
        result =  RTTI::Value(make_int4(be_checked_numcast<i32>(m_value[0]),
                                        be_checked_numcast<i32>(m_value[1]),
                                        be_checked_numcast<i32>(m_value[2]),
                                        be_checked_numcast<i32>(m_value[3])));
    else if (be_typeid<bigint4>::type().isA(expectedType))
        result =  RTTI::Value(make_bigint4(be_checked_numcast<i64>(m_value[0]),
                                           be_checked_numcast<i64>(m_value[1]),
                                           be_checked_numcast<i64>(m_value[2]),
                                           be_checked_numcast<i64>(m_value[3])));
    else if (be_typeid<ushort4>::type().isA(expectedType))
        result =  RTTI::Value(make_ushort4(be_checked_numcast<u16>(m_value[0]),
                                           be_checked_numcast<u16>(m_value[1]),
                                           be_checked_numcast<u16>(m_value[2]),
                                           be_checked_numcast<u16>(m_value[3])));
    else if (be_typeid<uint4>::type().isA(expectedType))
        result =  RTTI::Value(make_uint4(be_checked_numcast<u32>(m_value[0]),
                                         be_checked_numcast<u32>(m_value[1]),
                                         be_checked_numcast<u32>(m_value[2]),
                                         be_checked_numcast<u32>(m_value[3])));
    else if (be_typeid<biguint4>::type().isA(expectedType))
        result =  RTTI::Value(make_biguint4(be_checked_numcast<u64>(m_value[0]),
                                            be_checked_numcast<u64>(m_value[1]),
                                            be_checked_numcast<u64>(m_value[2]),
                                            be_checked_numcast<u64>(m_value[3])));
}

}}}
