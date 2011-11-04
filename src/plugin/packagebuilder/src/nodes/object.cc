/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#include    <stdafx.h>
#include    <packagebuilder/nodes/object.hh>
#include    <packagebuilder/nodes/package.hh>
#include    <packagebuilder/nodes/reference.hh>

namespace BugEngine { namespace PackageBuilder { namespace Nodes
{

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
        }
    }
}

}}}
