/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#include    <system/stdafx.h>
#include    <system/resource/resource.script.hh>
#include    <system/resource/resourceloader.hh>

namespace BugEngine
{

Resource::Resource()
{
}

Resource::~Resource()
{
}

void Resource::load(weak<const IResourceLoader> loader) const
{
    ResourceHandle* handle = 0;
    for(int i = 0; i < MaxResourceCount; ++i)
    {
        if (m_handles[i].owner == loader->m_loader)
        {
            be_warning("resource already loaded; skipping");
            return;
        }
        else if(m_handles[i].owner == 0)
        {
            handle = &m_handles[i];
        }
    }
    if (handle)
    {
        handle->owner = loader->m_loader;
        handle->resource = loader->load(this);
    }
    else
    {
        be_error("can't load resource: handles are all used");
    }
}

void Resource::unload(weak<const IResourceLoader> loader) const
{
    for(int i = 0; i < MaxResourceCount; ++i)
    {
        if (m_handles[i].owner == loader->m_loader)
        {
            loader->unload(m_handles[i].resource);
            m_handles[i].owner = 0;
            m_handles[i].resource = 0;
            return;
        }
    }
}

void* Resource::getResource(weak<const minitl::pointer> owner) const
{
    for(int i = 0; i < MaxResourceCount; ++i)
    {
        if (m_handles[i].owner == owner)
        {
            return m_handles[i].resource;
        }
    }
    return 0;
}

void Resource::load(const Value& v)
{
    be_assert_recover(be_typeid<const Resource>::type() <= v.type(), "not a resource to load, skipping", return);
    Value resourceloaders = v.type().metaclass->getTag<ResourceLoaders>();
    be_assert_recover(resourceloaders, "no resource loader on type %s" | v.type().name(), return);
    resourceloaders.as<const ResourceLoaders&>().load(v);
}

void Resource::unload(const Value& v)
{
    be_assert_recover(be_typeid<const Resource>::type() <= v.type(), "not a resource to load, skipping", return);
    Value resourceloaders = v.type().metaclass->getTag<ResourceLoaders>();
    be_assert_recover(resourceloaders, "no resource loader on type %s" | v.type().name(), return);
    resourceloaders.as<const ResourceLoaders&>().unload(v);
}

ResourceLoaders::ResourceLoaders()
    :   m_loaders(rttiArena())
{
}

ResourceLoaders::~ResourceLoaders()
{
}

void ResourceLoaders::add(ref<const IResourceLoader> loader)
{
    for (minitl::vector< ref<const IResourceLoader> >::const_iterator it = m_loaders.begin(); it != m_loaders.end(); ++it)
    {
        if ((*it)->m_loader == loader->m_loader)
        {
            be_error("added loader twice");
            return;
        }
    }
    m_loaders.push_back(loader);
}

void ResourceLoaders::remove(weak<const minitl::pointer> owner)
{
    for (minitl::vector< ref<const IResourceLoader> >::iterator it = m_loaders.begin(); it != m_loaders.end(); ++it)
    {
        if ((*it)->m_loader == owner)
        {
            m_loaders.erase(it);
            return;
        }
    }
    be_error("loader was not in the list of loaders for this type");
}

void ResourceLoaders::load(const Value& resource) const
{
    if (m_loaders.empty())
    {
        be_error("no loader was added for resource of type %s" | resource.type().name());
        return;
    }
    const Resource& r = resource.as<const Resource&>();
    for (minitl::vector< ref<const IResourceLoader> >::const_iterator it = m_loaders.begin(); it != m_loaders.end(); ++it)
    {
        r.load(*it);
    }
}

void ResourceLoaders::unload(const Value& resource) const
{
    if (m_loaders.empty())
    {
        be_error("no loader was added for resource of type %s" | resource.type().name());
        return;
    }
    const Resource& r = resource.as<const Resource&>();
    for (minitl::vector< ref<const IResourceLoader> >::const_iterator it = m_loaders.begin(); it != m_loaders.end(); ++it)
    {
        r.unload(*it);
    }
}

}
