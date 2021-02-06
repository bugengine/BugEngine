/* BugEngine <bugengine.devel@gmail.com>
   see LICENSE for detail */

#include <bugengine/plugin/stdafx.h>
#include <bugengine/plugin/dynobject.hh>

namespace BugEngine { namespace Plugin {

DynamicObject::DynamicObject(const inamespace& objectName, const ipath& objectPath)
    : m_handle(load(objectName, objectPath))
{
}

DynamicObject::~DynamicObject()
{
    if(m_handle)
    {
        unload(m_handle);
    }
}

}}  // namespace BugEngine::Plugin
