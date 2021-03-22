/* BugEngine <bugengine.devel@gmail.com>
   see LICENSE for detail */

#include <bugengine/introspect/stdafx.h>
#include <bugengine/introspect/node/node.hh>

#include <bugengine/introspect/dbcontext.hh>

namespace BugEngine { namespace Meta { namespace AST {

static const Value s_notFound = Value();

void Node::Visitor::accept(weak< const Array > arrayValue)
{
    be_forceuse(arrayValue);
}

void Node::Visitor::accept(weak< const Bool > boolValue)
{
    be_forceuse(boolValue);
}

void Node::Visitor::accept(weak< const FileName > filenameValue)
{
    be_forceuse(filenameValue);
}

void Node::Visitor::accept(weak< const Float > floatValue)
{
    be_forceuse(floatValue);
}

void Node::Visitor::accept(weak< const Float2 > float2Value)
{
    be_forceuse(float2Value);
}

void Node::Visitor::accept(weak< const Float3 > float3Value)
{
    be_forceuse(float3Value);
}

void Node::Visitor::accept(weak< const Float4 > float4Value)
{
    be_forceuse(float4Value);
}

void Node::Visitor::accept(weak< const Integer > integerValue)
{
    be_forceuse(integerValue);
}

void Node::Visitor::accept(weak< const Int2 > int2Value)
{
    be_forceuse(int2Value);
}

void Node::Visitor::accept(weak< const Int3 > int3Value)
{
    be_forceuse(int3Value);
}

void Node::Visitor::accept(weak< const Int4 > int4Value)
{
    be_forceuse(int4Value);
}

void Node::Visitor::accept(weak< const Object > objectValue)
{
    be_forceuse(objectValue);
}

void Node::Visitor::accept(weak< const Parameter > parameter, istring name,
                           weak< const Node > value)
{
    be_forceuse(parameter);
    be_forceuse(name);
    be_forceuse(value);
}

void Node::Visitor::accept(weak< const Property > propertyValue)
{
    be_forceuse(propertyValue);
}

void Node::Visitor::accept(weak< const Reference > referenceValue, const Value& resolvedValue)
{
    be_forceuse(referenceValue);
    be_forceuse(resolvedValue);
}

void Node::Visitor::accept(weak< const Reference > referenceValue, weak< const Node > resolvedNode)
{
    be_forceuse(referenceValue);
    be_forceuse(resolvedNode);
}

void Node::Visitor::accept(weak< const String > stringValue)
{
    be_forceuse(stringValue);
}

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
        return false;
    }
    else if(m_state == Resolved)
        return true;
    else if(m_state == ResolvedError)
        return false;
    else
    {
        m_state     = InResolution;
        bool result = doResolve(context);
        m_state     = result ? Resolved : ResolvedError;
        return result;
    }
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
    else
    {
        result = m_cache;
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

minitl::tuple< raw< const Meta::Method >, bool > Node::getCall(DbContext& context) const
{
    be_forceuse(context);
    return minitl::make_tuple(raw< const Meta::Method >::null(), false);
}

ref< Node > Node::getProperty(DbContext& context, const inamespace& name) const
{
    be_forceuse(name);
    context.error(this, Message::MessageType("object does not have properties"));
    return ref< Node >();
}

void Node::visit(Node::Visitor& visitor) const
{
    be_assert(m_state >= Resolved, "node is visited but has not yet been resolved");
    doVisit(visitor);
}

}}}  // namespace BugEngine::Meta::AST
