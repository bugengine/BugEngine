/* BugEngine / 2008-2012  Nicolas MERCIER <mercier.nicolas@gmail.com>
   see LICENSE for detail */

#include    <package/stdafx.h>
#include    <package/nodes/reference.hh>
#include    <package/nodes/object.hh>
#include    <package/nodes/package.hh>

#include    <rtti/value.hh>
#include    <rtti/engine/propertyinfo.script.hh>


namespace BugEngine { namespace PackageBuilder { namespace Nodes
{

Reference::Reference(weak<Package> owner)
    :   m_owner(owner)
    ,   m_name("")
    ,   m_value()
{
    m_owner->addReference(this);
}

Reference::~Reference()
{
}

void Reference::setName(const inamespace& name)
{
    m_name = name;
    m_owner->resolveReference(this);
}

RTTI::Value Reference::getValue() const
{
    if (m_value)
    {
        return m_value;
    }
    else if (m_object)
    {
        RTTI::Value v = m_owner->getValue(m_object);
        for (u32 i = 1; i < m_name.size(); ++i)
        {
            v = v[m_name[i]];
        }
        return v;
    }
    else
    {
        be_notreached();
        return m_value;
    }
}

RTTI::Type Reference::getType() const
{
    if (m_value)
    {
        return m_value.type();
    }
    else if (m_object)
    {
        RTTI::Type t = m_object->getType();
        for (u32 i = 1; i < m_name.size(); ++i)
        {
            raw<const RTTI::Property> p = t.metaclass->properties;
            while (p && p->name != m_name[i])
            {
                p = p->next;
            }
            if (!p)
            {
                be_notreached();
                return be_typeid<void>::type();
            }
            t = p->type;
        }
        return t;
    }
    else
    {
        be_notreached();
        return be_typeid<void>::type();
    }
}

}}}
