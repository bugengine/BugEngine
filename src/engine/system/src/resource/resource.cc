/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#include    <system/stdafx.h>
#include    <system/resource/resource.hh>


namespace BugEngine
{

Resource::ResourceHandle::ResourceHandle(weak<const IResourceManager> manager, const void* handle)
:   m_manager(manager)
,   m_handle(handle)
{
}

Resource::ResourceHandle::~ResourceHandle()
{
    m_manager->unloadResource(m_handle);
}

Resource::Resource()
:   m_handles()
{
}

Resource::~Resource()
{
}

}
