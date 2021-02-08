/* BugEngine <bugengine.devel@gmail.com>
   see LICENSE for detail */

#include <bugengine/rtti-ast/stdafx.h>
#include <bugengine/rtti-ast/node/object.hh>
#include <bugengine/rtti-ast/node/reference.hh>

#include <bugengine/rtti/classinfo.script.hh>
#include <bugengine/rtti/value.hh>

namespace BugEngine { namespace RTTI { namespace AST {

Parameter::Parameter(istring name, ref< Node > value) : m_name(name), m_value(value)
{
}

Parameter::~Parameter()
{
}

bool Parameter::resolve(DbContext& context)
{
    return m_value->resolve(context);
}

void Parameter::doEval(const Type& expectedType, Value& result) const
{
    m_value->doEval(expectedType, result);
}

bool Parameter::isCompatible(const Type& expectedType) const override
{
    return m_value->isCompatible(expectedType);
}

Object::Object(ref< Reference > className, const minitl::vector< ref< Parameter > >& parameters)
    : Node()
    , m_className(className)
    , m_parameters(parameters)

{
}

Object::~Object()
{
}

bool Object::resolve(DbContext& context)
{
    bool result = m_className->resolve(context);
    for(minitl::vector< ref< Parameter > >::const_iterator it = m_parameters.begin();
        it != m_parameters.end(); ++it)
    {
        result &= (*it)->resolve(context);
    }
    return result;
}

bool Object::isCompatible(const Type& expectedType) const
{
    be_forceuse(expectedType);
    be_unimplemented();
    return true;
}

void Object::doEval(const Type& expectedType, Value& result) const
{
    be_forceuse(expectedType);
    be_forceuse(result);
    be_unimplemented();
}

Type Object::getType() const
{
    return be_type< void >();
}

}}}  // namespace BugEngine::RTTI::AST
