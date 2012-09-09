/* BugEngine / 2008-2012  Nicolas MERCIER <mercier.nicolas@gmail.com>
   see LICENSE for detail */

#include    <plugin/stdafx.h>
#include    <plugin/dynobject.hh>

namespace BugEngine { namespace Plugin
{

DynamicObject::DynamicObject(const inamespace& objectName, const ipath& objectPath)
    :   m_handle(load(objectName, objectPath))
    ,   m_refCount(new (Arena::general()) i_u32(i_u32::One))
{
}

DynamicObject::DynamicObject(const DynamicObject& other)
    :   m_handle(other.m_handle)
    ,   m_refCount(other.m_refCount)
{
    ++*m_refCount;
}

DynamicObject::~DynamicObject()
{
    if (--*m_refCount == 0)
    {
        unload(m_handle);
        Arena::general().free(m_refCount);
    }
}

}}


