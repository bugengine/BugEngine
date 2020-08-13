/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#include <bugengine/plugin.scripting.package/stdafx.h>
#include <bugengine/plugin.scripting.package/nodes/object.hh>
#include <bugengine/plugin.scripting.package/nodes/package.hh>
#include <bugengine/plugin.scripting.package/nodes/parameter.hh>
#include <bugengine/plugin.scripting.package/nodes/reference.hh>

namespace BugEngine { namespace PackageBuilder { namespace Nodes {

Object::Object(weak< Package > owner, u32 line, u32 begin, u32 end)
    : Instance(owner, line, begin, end)
    , m_method()
    , m_overloads(Arena::packageBuilder())
{
}

Object::~Object()
{
}

void Object::addedParameter(ref< const Parameter > parameter)
{
    be_forceuse(parameter);
    for(minitl::vector< OverloadMatch >::iterator it = m_overloads.begin(); it != m_overloads.end();
        ++it)
    {
        it->update(m_parameters);
    }
    minitl::sort(m_overloads.begin(), m_overloads.end(), minitl::less< OverloadMatch >());
}

void Object::setMethod(ref< Reference > reference)
{
    m_methodReference = reference;
    RTTI::Value v(RTTI::Value::ByRef(m_methodReference->getValue()));
    if(v)
    {
        static const istring callName(RTTI::Class::nameOperatorCall());
        RTTI::Value          call = v[callName];
        if(call)
        {
            if(call.isA(be_type< raw< const RTTI::Method > const >()))
            {
                m_method = call.as< raw< const RTTI::Method > const >();
                if(m_method)
                {
                    m_overloads.clear();
                    m_overloads.reserve(m_method->overloads.count);
                    for(const RTTI::Method::Overload* o = m_method->overloads.begin();
                        o != m_method->overloads.end(); ++o)
                    {
                        raw< const RTTI::Method::Overload > overload = {o};
                        m_overloads.push_back(OverloadMatch(overload));
                        m_overloads.back().update(m_parameters);
                    }
                    minitl::sort(m_overloads.begin(), m_overloads.end(),
                                 minitl::less< OverloadMatch >());
                    if(m_overloads[0].m_callInfo.conversion >= RTTI::ConversionCost::s_incompatible)
                    {
                        // error: param list incorrect
                        m_owner->error(m_line, minitl::format< 1024 >(
                                                   "no overload for method %s could be found")
                                                   | m_methodReference->name());
                        for(minitl::vector< OverloadMatch >::const_iterator it
                            = m_overloads.begin();
                            it != m_overloads.end(); ++it)
                        {
                            m_owner->info(m_line, minitl::format< 1024 >(" candidate: %s(%s)")
                                                      | m_methodReference->name()
                                                      | it->m_callInfo.overload->signature());
                        }
                        m_overloads.clear();
                    }
                }
                else
                {
                    // error: call method is null
                    m_owner->error(m_line,
                                   minitl::format< 1024 >("%s does not have any constructor")
                                       | m_methodReference->name());
                }
            }
            else
            {
                // error: call is not a method
                m_owner->error(m_line, minitl::format< 1024 >("%s has an invalid call object")
                                           | m_methodReference->name());
            }
        }
        else
        {
            // error: no call
            m_owner->error(m_line, minitl::format< 1024 >("%s is not a callable object")
                                       | m_methodReference->name());
        }
    }
    else
    {
        // error: no value, it should already have been reported by the reference
    }
}

RTTI::Type Object::getType() const
{
    if(m_overloads.empty())
    {
        return be_type< void >();
    }
    else
    {
        return m_overloads[0].m_callInfo.overload->returnType;
    }
}

RTTI::Value Object::create() const
{
    return m_overloads.empty() ? RTTI::Value() : m_overloads[0].create(m_name);
}

}}}  // namespace BugEngine::PackageBuilder::Nodes
