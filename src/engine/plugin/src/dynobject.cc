/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#include    <plugin/stdafx.h>
#include    <plugin/dynobject.hh>

namespace BugEngine { namespace Plugin
{

DynamicObject::DynamicObject(const inamespace& objectName, const ipath& objectPath)
    :   m_handle(load(objectName, objectPath))
{
}

DynamicObject::~DynamicObject()
{
    if(m_handle)
    {
        unload(m_handle);
    }
}

}}


