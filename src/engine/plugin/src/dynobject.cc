/* BugEngine / 2008-2012  Nicolas MERCIER <mercier.nicolas@gmail.com>
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
    unload(m_handle);
}

}}


