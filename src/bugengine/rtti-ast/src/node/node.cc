/* BugEngine <bugengine.devel@gmail.com>
   see LICENSE for detail */

#include <bugengine/rtti-ast/stdafx.h>
#include <bugengine/rtti-ast/node/node.hh>

#include <bugengine/rtti-ast/dbcontext.hh>

namespace BugEngine { namespace RTTI { namespace AST {

static const Value s_notFound = Value();

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
            context.error(this,
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
            context.error(this,
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

const Value& Node::getMetadata(const istring name) const
{
    for(minitl::vector< minitl::tuple< const istring, Value > >::const_iterator it
        = m_metadata.begin();
        it != m_metadata.end(); ++it)
    {
        if(it->first == name) return it->second;
    }
    return s_notFound;
}

}}}  // namespace BugEngine::RTTI::AST
