/* BugEngine <bugengine.devel@gmail.com>
   see LICENSE for detail */

#include <bugengine/rtti-ast/stdafx.h>
#include <bugengine/rtti-ast/node/integer.hh>

#include <bugengine/rtti-ast/dbcontext.hh>

namespace BugEngine { namespace RTTI { namespace AST {

Integer::Integer(i64 value) : Node(), m_value(value)
{
    be_forceuse(m_value);
}

Integer::~Integer()
{
}

ConversionCost Integer::distance(const Type& type) const
{
    return ConversionCalculator< i64 >::calculate(type);
}

void Integer::doEval(const RTTI::Type& expectedType, RTTI::Value& result) const
{
    if(be_type< i8 >().isA(expectedType))
        result = RTTI::Value(be_checked_numcast< i8 >(m_value));
    else if(be_type< i16 >().isA(expectedType))
        result = RTTI::Value(be_checked_numcast< i16 >(m_value));
    else if(be_type< i32 >().isA(expectedType))
        result = RTTI::Value(be_checked_numcast< i32 >(m_value));
    else if(be_type< i64 >().isA(expectedType))
        result = RTTI::Value(be_checked_numcast< i64 >(m_value));
    else if(be_type< u8 >().isA(expectedType))
        result = RTTI::Value(be_checked_numcast< u8 >(m_value));
    else if(be_type< u16 >().isA(expectedType))
        result = RTTI::Value(be_checked_numcast< u16 >(m_value));
    else if(be_type< u32 >().isA(expectedType))
        result = RTTI::Value(be_checked_numcast< u32 >(m_value));
    else if(be_type< u64 >().isA(expectedType))
        result = RTTI::Value(be_checked_numcast< u64 >(m_value));
    else
        be_notreached();
}

void Integer::doVisit(Node::Visitor& visitor) const
{
    visitor.accept(this);
}

}}}  // namespace BugEngine::RTTI::AST
