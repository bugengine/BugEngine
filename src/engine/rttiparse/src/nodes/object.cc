/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#include    <rttiparse/stdafx.h>
#include    <rttiparse/object.hh>
#include    <rtti/value.hh>
#include    <rtti/classinfo.script.hh>


namespace BugEngine { namespace RTTI { namespace Parser
{

struct ParameterMatch
{
    raw<const RTTI::Method::Parameter>  param;
    Parameter*                          match;
};

static raw<const RTTI::Method::Overload> findMatch(DbContext& context,
                                                   raw<const Method> m,
                                                   const minitl::vector<Parameter>& parameters)
{
    be_forceuse(context);
    raw<const RTTI::Method::Overload> result;
    for (raw<const RTTI::Method::Overload> o = m->overloads; o; o = o->next)
    {
        ParameterMatch* matches = (ParameterMatch*)malloca(o->parameterCount * sizeof(ParameterMatch));
        u32 i = 0;
        //u32 unused = parameters.size();
        for (raw<const RTTI::Method::Parameter> p = o->params; p; p = p->next, ++i)
        {
            for (minitl::vector<Parameter>::const_iterator it = parameters.begin();
                 it != parameters.end();
                 ++it)
            {
                if (it->name == p->name)
                {

                    break;
                }
            }
        }
        freea(matches);
    }
    return result;
}

Object::Object(const ParseLocation& location, ref<Reference> className,
               const minitl::vector<Parameter>& parameters)
    :   Node(location)
    ,   m_className(className)
    ,   m_parameters(parameters)

{
    be_forceuse(findMatch);
}

Object::~Object()
{
}

bool Object::resolve(DbContext& context)
{
    bool result = m_className->resolve(context);
    for (minitl::vector<Parameter>::const_iterator it = m_parameters.begin();
         it != m_parameters.end();
         ++it)
    {
        result &= it->value->resolve(context);
    }
    return result;
}

bool Object::isCompatible(const Type &expectedType) const
{
    be_forceuse(expectedType);
    return true;
}

void Object::doEval(const Type &expectedType, Value &result) const
{
    be_forceuse(expectedType);
    be_forceuse(result);
}

Type Object::getType() const
{
    return be_typeid<void>::type();
}

}}}
