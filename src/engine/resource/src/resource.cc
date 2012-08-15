/* BugEngine / 2008-2012  Nicolas MERCIER <mercier.nicolas@gmail.com>
   see LICENSE for detail */

#include    <resource/stdafx.h>
#include    <resource/resource.script.hh>
#include    <resource/resourceloader.hh>

namespace BugEngine
{

Resource::Resource()
{
}

Resource::~Resource()
{
}

const ResourceHandle& Resource::getResourceHandle(weak<const IResourceLoader> owner) const
{
    for(int i = 0; i < MaxResourceCount; ++i)
    {
        if (m_handles[i].first == owner)
        {
            return m_handles[i].second;
        }
    }
    return ResourceHandle::null();
}

void Resource::load(weak<IResourceLoader> loader) const
{
    for(int i = 0; i < MaxResourceCount; ++i)
    {
        if (!m_handles[i].first)
        {
            m_handles[i].first = loader;
            m_handles[i].second = loader->load(this);
            return;
        }
    }
    be_notreached();
}

void Resource::unload(weak<const IResourceLoader> loader) const
{
    for(int i = 0; i < MaxResourceCount; ++i)
    {
        if (m_handles[i].first == loader)
        {
            m_handles[i].first->unload(m_handles[i].second);
            m_handles[i].first = weak<IResourceLoader>();
            m_handles[i].second = ResourceHandle();
            return;
        }
    }
    be_notreached();
}

ResourceHandle& Resource::getResourceHandleForWriting(weak<const IResourceLoader> owner) const
{
    for(int i = 0; i < MaxResourceCount; ++i)
    {
        if (m_handles[i].first == owner)
        {
            return m_handles[i].second;
        }
    }
    return ResourceHandle::null();
}

}