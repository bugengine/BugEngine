/* BugEngine <bugengine.devel@gmail.com>
   see LICENSE for detail */

#include <bugengine/rtti-ast/stdafx.h>
#include <bugengine/rtti-ast/node/node.hh>

namespace BugEngine { namespace RTTI { namespace AST {

bool Node::resolve(DbContext& context)
{
    be_forceuse(context);
    return true;
}

void Node::eval(DbContext& context, const Type& expectedType, Value& result) const
{
    if(!m_cacheSet)
    {
        if(!isCompatible(expectedType))
        {
            context.error(m_location,
                          Message::MessageType("type mismatch; expected %s") | expectedType.name());
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
    if(!m_cacheSet)
    {
        if(!isCompatible(expectedType))
        {
            context.error(m_location,
                          Message::MessageType("type mismatch; expected %s") | expectedType.name());
        }
        else
        {
            doEval(expectedType, m_cache);
        }
        m_cacheSet = true;
    }
    return Value(Value::ByRef(m_cache));
}

}}}  // namespace BugEngine::RTTI::AST
