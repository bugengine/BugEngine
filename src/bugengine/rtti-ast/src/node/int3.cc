/* BugEngine <bugengine.devel@gmail.com>
   see LICENSE for detail */

#include <bugengine/rtti-ast/stdafx.h>
#include <bugengine/rtti-ast/node/int3.hh>

#include <bugengine/rtti-ast/dbcontext.hh>

namespace BugEngine { namespace RTTI { namespace AST {

Int3::Int3(bigint3 value) : Node(), m_value(value)
{
    be_forceuse(m_value);
}

Int3::~Int3()
{
}

ConversionCost Int3::distance(const Type& type) const
{
    return ConversionCalculator< bigint3 >::calculate(type);
}

void Int3::doEval(const RTTI::Type& expectedType, Value& result) const
{
    if(be_type< byte3 >().isA(expectedType))
        result = RTTI::Value(make_byte3(be_checked_numcast< i8 >(m_value[0]),
                                        be_checked_numcast< i8 >(m_value[1]),
                                        be_checked_numcast< i8 >(m_value[2])));
    else if(be_type< short3 >().isA(expectedType))
        result = RTTI::Value(make_short3(be_checked_numcast< i16 >(m_value[0]),
                                         be_checked_numcast< i16 >(m_value[1]),
                                         be_checked_numcast< i16 >(m_value[2])));
    else if(be_type< int3 >().isA(expectedType))
        result = RTTI::Value(make_int3(be_checked_numcast< i32 >(m_value[0]),
                                       be_checked_numcast< i32 >(m_value[1]),
                                       be_checked_numcast< i32 >(m_value[2])));
    else if(be_type< bigint3 >().isA(expectedType))
        result = RTTI::Value(make_bigint3(be_checked_numcast< i64 >(m_value[0]),
                                          be_checked_numcast< i64 >(m_value[1]),
                                          be_checked_numcast< i64 >(m_value[2])));
    else if(be_type< ushort3 >().isA(expectedType))
        result = RTTI::Value(make_ushort3(be_checked_numcast< u16 >(m_value[0]),
                                          be_checked_numcast< u16 >(m_value[1]),
                                          be_checked_numcast< u16 >(m_value[2])));
    else if(be_type< uint3 >().isA(expectedType))
        result = RTTI::Value(make_uint3(be_checked_numcast< u32 >(m_value[0]),
                                        be_checked_numcast< u32 >(m_value[1]),
                                        be_checked_numcast< u32 >(m_value[2])));
    else if(be_type< biguint3 >().isA(expectedType))
        result = RTTI::Value(make_biguint3(be_checked_numcast< u64 >(m_value[0]),
                                           be_checked_numcast< u64 >(m_value[1]),
                                           be_checked_numcast< u64 >(m_value[2])));
    else
        be_notreached();
}

void Int3::doVisit(Node::Visitor& visitor) const
{
    visitor.accept(this);
}

}}}  // namespace BugEngine::RTTI::AST
