/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#include    <stdafx.h>
#include    <packagebuilder/nodes/object.hh>
#include    <packagebuilder/nodes/package.hh>
#include    <packagebuilder/nodes/reference.hh>
#include    <packagebuilder/nodes/parameter.hh>

namespace BugEngine { namespace PackageBuilder { namespace Nodes
{

struct ParameterMatch
{
    typedef RTTI::MethodInfo::OverloadInfo::ParamInfo ParamInfo;
    minitl::vector< ref<Parameter> >                                    unused;
    minitl::vector< const ParamInfo* >                                  missing;
    minitl::vector< minitl::pair< ref<Parameter>, const ParamInfo*> >   matches;
    u32                                                                 match;
    ParameterMatch(const RTTI::MethodInfo::OverloadInfo* overload, const minitl::vector< ref<Parameter> >& parameters)
        :   unused(tempArena())
        ,   missing(tempArena())
        ,   matches(tempArena())
        ,   match(0)
    {
        minitl::vector< ref<Parameter> > params(tempArena(), parameters.begin(), parameters.end());
        for (const ParamInfo* param = overload->params; param; param = param->next)
        {
            bool matched = false;
            for (minitl::vector< ref<Parameter> >::iterator value = params.begin(); value != params.end(); ++value)
            {
                if (param->name == (*value)->name())
                {
                    matches.push_back(minitl::make_pair(*value, param));
                    matched = true;
                    params.erase(value);
                    break;
                }
            }
            if (!matched)
            {
                missing.push_back(param);
            }
        }
    }

    bool operator<(const ParameterMatch& other)
    {
        return match < other.match;
    }
};


Object::Object(weak<Package> owner)
    :   m_owner(owner)
    ,   m_name("")
    ,   m_method(0)
    ,   m_overload(0)
    ,   m_parameters(packageBuilderArena())
{
}

Object::~Object()
{
}

void Object::setName(istring name)
{
    if (m_owner->findByName(name))
    {
        // error
    }
    m_name = name;
}

void Object::setMethod(ref<Reference> reference)
{
    m_methodReference = reference;
    Value v(Value::ByRef(m_methodReference->value()));
    if (v)
    {
        static const istring callName("call");
        Value call = v[callName];
        if (call && be_typeid<const RTTI::MethodInfo* const>::type() <= call.type())
        {
            m_method = call.as<const RTTI::MethodInfo* const>();
            resolveOverload();
        }
        else if (call)
        {
            // error
        }
        else
        {
            // error
        }
    }
}

void Object::resolveOverload()
{
    m_overload = 0;
    if (m_method)
    {
    }
}

void Object::doCall()
{
    be_assert(m_method, "no method");
    be_assert(m_overload, "no proper overload");
    m_overload->call(0, 0);
}

}}}
