/* BugEngine <bugengine.devel@gmail.com>
   see LICENSE for detail */

#include <bugengine/introspect/stdafx.h>
#include <bugengine/introspect/node/parameter.hh>

namespace BugEngine { namespace Meta { namespace AST {

Parameter::Parameter(const istring name, ref< Node > value) : m_name(name), m_value(value)
{
}

Parameter::~Parameter()
{
}

ConversionCost Parameter::distance(const Type& type) const
{
    return m_value->distance(type);
}

bool Parameter::doResolve(DbContext& context)
{
    return m_value->resolve(context);
}

void Parameter::doEval(const Type& expectedType, Value& result) const
{
    return m_value->eval(expectedType, result);
}

void Parameter::doVisit(Node::Visitor& visitor) const
{
    visitor.accept(this, m_name, m_value);
}

}}}  // namespace BugEngine::Meta::AST
