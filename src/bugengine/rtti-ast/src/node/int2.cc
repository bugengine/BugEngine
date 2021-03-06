/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#include <bugengine/rtti-ast/stdafx.h>
#include <bugengine/rtti-ast/node/int2.hh>

namespace BugEngine { namespace RTTI { namespace AST {

Int2::Int2(const ParseLocation& location, bigint2 value) : Node(location), m_value(value)
{
    be_forceuse(m_value);
}

Int2::~Int2()
{
}

bool Int2::isCompatible(const RTTI::Type& expectedType) const
{
    return be_type< byte2 >().isA(expectedType) || be_type< short2 >().isA(expectedType)
           || be_type< int2 >().isA(expectedType) || be_type< bigint2 >().isA(expectedType)
           || be_type< ushort2 >().isA(expectedType) || be_type< uint2 >().isA(expectedType)
           || be_type< biguint2 >().isA(expectedType);
}

void Int2::doEval(const RTTI::Type& expectedType, Value& result) const
{
    be_assert(isCompatible(expectedType), "invalid conversion from int2 to %s" | expectedType);
    if(be_type< byte2 >().isA(expectedType))
        result = RTTI::Value(
            make_byte2(be_checked_numcast< i8 >(m_value[0]), be_checked_numcast< i8 >(m_value[1])));
    else if(be_type< short2 >().isA(expectedType))
        result = RTTI::Value(make_short2(be_checked_numcast< i16 >(m_value[0]),
                                         be_checked_numcast< i16 >(m_value[1])));
    else if(be_type< int2 >().isA(expectedType))
        result = RTTI::Value(make_int2(be_checked_numcast< i32 >(m_value[0]),
                                       be_checked_numcast< i32 >(m_value[1])));
    else if(be_type< bigint2 >().isA(expectedType))
        result = RTTI::Value(make_bigint2(be_checked_numcast< i64 >(m_value[0]),
                                          be_checked_numcast< i64 >(m_value[1])));
    else if(be_type< ushort2 >().isA(expectedType))
        result = RTTI::Value(make_ushort2(be_checked_numcast< u16 >(m_value[0]),
                                          be_checked_numcast< u16 >(m_value[1])));
    else if(be_type< uint2 >().isA(expectedType))
        result = RTTI::Value(make_uint2(be_checked_numcast< u32 >(m_value[0]),
                                        be_checked_numcast< u32 >(m_value[1])));
    else if(be_type< biguint2 >().isA(expectedType))
        result = RTTI::Value(make_biguint2(be_checked_numcast< u64 >(m_value[0]),
                                           be_checked_numcast< u64 >(m_value[1])));
}

}}}  // namespace BugEngine::RTTI::AST
