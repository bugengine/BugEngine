/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#include    <package/stdafx.h>
#include    <package/nodes/reference.hh>
#include    <package/nodes/object.hh>
#include    <package/nodes/package.hh>


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

const BugEngine::Value& Reference::getValue() const
{
    if (m_value)
    {
        return m_value;
    }
    else if (m_object)
    {
        return m_owner->getValue(m_object);
    }
    else
    {
        be_notreached();
        return m_value;
    }
}

Type Reference::getType() const
{
    if (m_value)
    {
        return m_value.type();
    }
    else if (m_object)
    {
        return m_object->getType();
    }
    else
    {
        be_notreached();
        return be_typeid<void>::type();
    }
}

}}}
