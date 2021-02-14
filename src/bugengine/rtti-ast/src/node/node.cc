/* BugEngine <bugengine.devel@gmail.com>
   see LICENSE for detail */

#include <bugengine/rtti-ast/stdafx.h>
#include <bugengine/rtti-ast/node/node.hh>

#include <bugengine/rtti-ast/dbcontext.hh>

namespace BugEngine { namespace RTTI { namespace AST {

static const Value s_notFound = Value();

bool Node::doResolve(DbContext& context)
{
    be_forceuse(context);
    return true;
}

bool Node::resolve(DbContext& context)
{
    be_assert(m_state != Evaluated, "node is already evaluated");
    if(m_state == InResolution)
    {
        context.error(this, Message::MessageType("circular reference detected"));
    }
    m_state     = InResolution;
    bool result = doResolve(context);
    m_state     = Resolved;
    return result;
}

void Node::eval(const Type& expectedType, Value& result) const
{
    if(m_state != Evaluated)
    {
        be_assert(m_state == Resolved, "node has skipped resolution stage");
        doEval(expectedType, result);
        m_cache = Value(Value::ByRef(result));
        m_state = Evaluated;
    }
}

Value Node::eval(const Type& expectedType) const
{
    if(m_state != Evaluated)
    {
        be_assert(m_state == Resolved, "node has skipped resolution stage");
        doEval(expectedType, m_cache);
        m_state = Evaluated;
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

minitl::tuple< raw< const RTTI::Method >, bool > Node::getCall(DbContext& context) const
{
    be_forceuse(context);
    return minitl::make_tuple(raw< const RTTI::Method >::null(), false);
}

}}}  // namespace BugEngine::RTTI::AST
