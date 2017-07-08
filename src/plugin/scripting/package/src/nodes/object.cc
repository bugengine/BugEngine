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

void Object::addedParameter(ref<Parameter> parameter)
{
    for (minitl::vector< OverloadMatch >::iterator it = m_overloads.begin(); it != m_overloads.end(); ++it)
    {
        it->addParameter(parameter);
    }
    minitl::sort(m_overloads.begin(), m_overloads.end(), minitl::less<OverloadMatch>());
}

void Object::setMethod(ref<Reference> reference)
{
    be_unimplemented();
    be_forceuse(reference);
}

RTTI::Type Object::getType() const
{
    if (m_overloads.empty())
    {
        be_notreached();
        return be_typeid<void>::type();
    }
    else
    {
        return m_overloads[0].m_overload->returnType;
    }
}

RTTI::Value Object::create() const
{
    return m_overloads.empty() ? RTTI::Value() : m_overloads[0].create(m_name);
}

}}}
