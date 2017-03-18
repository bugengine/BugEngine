/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#include    <rttiparse/stdafx.h>
#include    <rttiparse/node.hh>


namespace BugEngine { namespace RTTI { namespace Parser
{

bool Node::resolve(DbContext& context)
{
    be_forceuse(context);
    return true;
}

void Node::eval(DbContext &context, const Type &expectedType, Value& result) const
{
    if (!m_cacheSet)
    {
        if (!isCompatible(expectedType))
        {
            context.error(m_location,
                          ErrorType("type mismatch; expected %s") | expectedType.name());
        }
        else
        {
            doEval(expectedType, result);
            m_cache = Value(Value::ByRef(result));
        }
        m_cacheSet = true;
    }
}

Value Node::eval(DbContext& context, const Type& expectedType) const
{
    if (!m_cacheSet)
    {
        if (!isCompatible(expectedType))
        {
            context.error(m_location,
                          ErrorType("type mismatch; expected %s") | expectedType.name());
        }
        else
        {
            doEval(expectedType, m_cache);
        }
        m_cacheSet = true;
    }
    return Value(Value::ByRef(m_cache));
}

}}}
