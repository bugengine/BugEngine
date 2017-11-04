/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#include    <package/stdafx.h>
#include    <package/nodes/object.hh>
#include    <package/nodes/package.hh>
#include    <package/nodes/reference.hh>
#include    <package/nodes/parameter.hh>

namespace BugEngine { namespace PackageBuilder { namespace Nodes
{

Object::Object(weak<Package> owner, u32 line, u32 begin, u32 end)
    :   Instance(owner, line, begin, end)
    ,   m_method()
    ,   m_overloads(Arena::packageBuilder())
{
}

Object::~Object()
{
}

void Object::addedParameter(ref<const Parameter> parameter)
{
    be_forceuse(parameter);
    for (minitl::vector< OverloadMatch >::iterator it = m_overloads.begin(); it != m_overloads.end(); ++it)
    {
        it->update(m_parameters);
    }
    minitl::sort(m_overloads.begin(), m_overloads.end(), minitl::less<OverloadMatch>());
}

void Object::setMethod(ref<Reference> reference)
{
    m_methodReference = reference;
    RTTI::Value v(RTTI::Value::ByRef(m_methodReference->getValue()));
    if (v)
    {
        static const istring callName(RTTI::Class::nameOperatorCall());
        RTTI::Value call = v[callName];
        if (call)
        {
            if (call.isA(be_typeid<raw<const RTTI::Method> const>::type()))
            {
                m_method = call.as<raw<const RTTI::Method> const>();
                if (m_method)
                {
                    m_overloads.clear();
                    m_overloads.reserve(m_method->overloads.count);
                    for (const RTTI::Method::Overload* o = m_method->overloads.begin();
                         o != m_method->overloads.end();
                         ++o)
                    {
                        raw<const RTTI::Method::Overload> overload = { o };
                        m_overloads.push_back(OverloadMatch(overload));
                        m_overloads.back().update(m_parameters);
                    }
                    minitl::sort(m_overloads.begin(),
                                 m_overloads.end(),
                                 minitl::less<OverloadMatch>());
                    if (m_overloads[0].m_callInfo.conversion >= RTTI::ConversionCost::s_incompatible)
                    {
                        // error: param list incorrect
                        be_unimplemented();
                    }
                }
                else
                {
                    // error: call method is null
                    be_unimplemented();
                }
            }
            else
            {
                // error: call is not a method
                be_unimplemented();
            }
        }
        else
        {
            // error: no call
            be_unimplemented();
        }
    }
    else
    {
        // error: no value?
        be_unimplemented();
    }
}

RTTI::Type Object::getType() const
{
    if (m_overloads.empty())
    {
        return be_typeid<void>::type();
    }
    else
    {
        return m_overloads[0].m_callInfo.overload->returnType;
    }
}

RTTI::Value Object::create() const
{
    return m_overloads.empty() ? RTTI::Value()
                               : m_overloads[0].create(m_name);
}

}}}
