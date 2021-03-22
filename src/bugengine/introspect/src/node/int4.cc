/* BugEngine <bugengine.devel@gmail.com>
   see LICENSE for detail */

#include <bugengine/introspect/stdafx.h>
#include <bugengine/introspect/node/int4.hh>

#include <bugengine/introspect/dbcontext.hh>

namespace BugEngine { namespace Meta { namespace AST {

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

void Int4::doEval(const Meta::Type& expectedType, Value& result) const
{
    if(be_type< byte4 >().isA(expectedType))
        result = Meta::Value(
            make_byte4(be_checked_numcast< i8 >(m_value[0]), be_checked_numcast< i8 >(m_value[1]),
                       be_checked_numcast< i8 >(m_value[2]), be_checked_numcast< i8 >(m_value[3])));
    else if(be_type< short4 >().isA(expectedType))
        result = Meta::Value(make_short4(
            be_checked_numcast< i16 >(m_value[0]), be_checked_numcast< i16 >(m_value[1]),
            be_checked_numcast< i16 >(m_value[2]), be_checked_numcast< i16 >(m_value[3])));
    else if(be_type< int4 >().isA(expectedType))
        result = Meta::Value(make_int4(
            be_checked_numcast< i32 >(m_value[0]), be_checked_numcast< i32 >(m_value[1]),
            be_checked_numcast< i32 >(m_value[2]), be_checked_numcast< i32 >(m_value[3])));
    else if(be_type< bigint4 >().isA(expectedType))
        result = Meta::Value(make_bigint4(
            be_checked_numcast< i64 >(m_value[0]), be_checked_numcast< i64 >(m_value[1]),
            be_checked_numcast< i64 >(m_value[2]), be_checked_numcast< i64 >(m_value[3])));
    else if(be_type< ushort4 >().isA(expectedType))
        result = Meta::Value(make_ushort4(
            be_checked_numcast< u16 >(m_value[0]), be_checked_numcast< u16 >(m_value[1]),
            be_checked_numcast< u16 >(m_value[2]), be_checked_numcast< u16 >(m_value[3])));
    else if(be_type< uint4 >().isA(expectedType))
        result = Meta::Value(make_uint4(
            be_checked_numcast< u32 >(m_value[0]), be_checked_numcast< u32 >(m_value[1]),
            be_checked_numcast< u32 >(m_value[2]), be_checked_numcast< u32 >(m_value[3])));
    else if(be_type< biguint4 >().isA(expectedType))
        result = Meta::Value(make_biguint4(
            be_checked_numcast< u64 >(m_value[0]), be_checked_numcast< u64 >(m_value[1]),
            be_checked_numcast< u64 >(m_value[2]), be_checked_numcast< u64 >(m_value[3])));
    else
        be_notreached();
}

void Int4::doVisit(Node::Visitor& visitor) const
{
    visitor.accept(this);
}

}}}  // namespace BugEngine::Meta::AST
