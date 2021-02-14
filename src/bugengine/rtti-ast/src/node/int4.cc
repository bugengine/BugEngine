/* BugEngine <bugengine.devel@gmail.com>
   see LICENSE for detail */

#include <bugengine/rtti-ast/stdafx.h>
#include <bugengine/rtti-ast/node/int4.hh>

#include <bugengine/rtti-ast/dbcontext.hh>

namespace BugEngine { namespace RTTI { namespace AST {

Int4::Int4(bigint4 value) : Node(), m_value(value)
{
    be_forceuse(m_value);
}

Int4::~Int4()
{
}

ConversionCost Int4::distance(const Type& type) const
{
    return ConversionCalculator< bigint4 >::calculate(type);
}

bool Int4::isCompatible(DbContext& context, const RTTI::Type& expectedType) const
{
    if(!(be_type< byte4 >().isA(expectedType) || be_type< short4 >().isA(expectedType)
         || be_type< int4 >().isA(expectedType) || be_type< bigint4 >().isA(expectedType)
         || be_type< ushort4 >().isA(expectedType) || be_type< uint4 >().isA(expectedType)
         || be_type< biguint4 >().isA(expectedType)))
    {
        context.error(this,
                      Message::MessageType("cannot cast int4 value to %s") | expectedType.name());
        return false;
    }
    else
        return true;
}

void Int4::doEval(const RTTI::Type& expectedType, Value& result) const
{
    if(be_type< byte4 >().isA(expectedType))
        result = RTTI::Value(
            make_byte4(be_checked_numcast< i8 >(m_value[0]), be_checked_numcast< i8 >(m_value[1]),
                       be_checked_numcast< i8 >(m_value[2]), be_checked_numcast< i8 >(m_value[3])));
    else if(be_type< short4 >().isA(expectedType))
        result = RTTI::Value(make_short4(
            be_checked_numcast< i16 >(m_value[0]), be_checked_numcast< i16 >(m_value[1]),
            be_checked_numcast< i16 >(m_value[2]), be_checked_numcast< i16 >(m_value[3])));
    else if(be_type< int4 >().isA(expectedType))
        result = RTTI::Value(make_int4(
            be_checked_numcast< i32 >(m_value[0]), be_checked_numcast< i32 >(m_value[1]),
            be_checked_numcast< i32 >(m_value[2]), be_checked_numcast< i32 >(m_value[3])));
    else if(be_type< bigint4 >().isA(expectedType))
        result = RTTI::Value(make_bigint4(
            be_checked_numcast< i64 >(m_value[0]), be_checked_numcast< i64 >(m_value[1]),
            be_checked_numcast< i64 >(m_value[2]), be_checked_numcast< i64 >(m_value[3])));
    else if(be_type< ushort4 >().isA(expectedType))
        result = RTTI::Value(make_ushort4(
            be_checked_numcast< u16 >(m_value[0]), be_checked_numcast< u16 >(m_value[1]),
            be_checked_numcast< u16 >(m_value[2]), be_checked_numcast< u16 >(m_value[3])));
    else if(be_type< uint4 >().isA(expectedType))
        result = RTTI::Value(make_uint4(
            be_checked_numcast< u32 >(m_value[0]), be_checked_numcast< u32 >(m_value[1]),
            be_checked_numcast< u32 >(m_value[2]), be_checked_numcast< u32 >(m_value[3])));
    else if(be_type< biguint4 >().isA(expectedType))
        result = RTTI::Value(make_biguint4(
            be_checked_numcast< u64 >(m_value[0]), be_checked_numcast< u64 >(m_value[1]),
            be_checked_numcast< u64 >(m_value[2]), be_checked_numcast< u64 >(m_value[3])));
    else
        be_notreached();
}

}}}  // namespace BugEngine::RTTI::AST
